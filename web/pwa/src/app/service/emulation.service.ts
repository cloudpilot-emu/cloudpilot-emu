import { ApplicationRef, Injectable, NgZone } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { StorageCardProvider } from '@common/engine/Engine';
import { EngineSettings } from '@common/engine/EngineSettings';
import { SnapshotContainer } from '@common/engine/Snapshot';
import { isIOS } from '@common/helper/browser';
import { SchedulerKind } from '@common/helper/scheduler';
import { AbstractEmulationService, Executor } from '@common/service/AbstractEmulationService';
import { LoadingController } from '@ionic/angular';
import { Mutex } from 'async-mutex';

import { clearStoredSession, getStoredSession, setStoredSession } from '@pwa/helper/storedSession';
import { Session } from '@pwa/model/Session';
import { StorageCardService } from '@pwa/service/storage-card.service';

import { AlertService } from './alert.service';
import { BootstrapService } from './bootstrap-service';
import { ButtonService } from './button.service';
import { ClipboardService } from './clipboard.service';
import { CloudpilotService } from './cloudpilot.service';
import { EmulationContextService } from './emulation-context.service';
import { ErrorService } from './error.service';
import { FeatureService } from './feature.service';
import { KvsService } from './kvs.service';
import { hasInitialImportRequest } from './link-api.service';
import { ModalWatcherService } from './modal-watcher.service';
import { NetworkService } from './network.service';
import { SessionService } from './session.service';
import { SnapshotService } from './snapshot.service';
import { CardOwner, StorageCardContext } from './storage-card-context';
import { StorageService } from './storage.service';

// TODO: Snapshots
// TODO: ModalWatcher
// TODO: Insert / Eject storage (badly broken atm)
// TODO: Get rid of cloudpilot service

@Injectable({ providedIn: 'root' })
export class EmulationService extends AbstractEmulationService {
    constructor(
        private storageService: StorageService,
        private ngZone: NgZone,
        private loadingController: LoadingController,
        private emulationContext: EmulationContextService,
        private snapshotService: SnapshotService,
        private errorService: ErrorService,
        private alertService: AlertService,
        private modalWatcher: ModalWatcherService,
        private clipboardService: ClipboardService,
        private kvsService: KvsService,
        private networkService: NetworkService,
        private buttonService: ButtonService,
        private bootstrapService: BootstrapService,
        private storageCardService: StorageCardService,
        private storageCardContext: StorageCardContext,
        private sessionsService: SessionService,
        private featureService: FeatureService,
        private cloudpilotService: CloudpilotService,
        private app: ApplicationRef,
    ) {
        super();

        storageService.sessionChangeEvent.addHandler(this.onSessionChange);
        errorService.fatalErrorEvent.addHandler(this.stop);
        this.alertService.emergencySaveEvent.addHandler(this.onEmergencySave);

        void this.getCloudpilotInstance().then((instance) => {
            this.cloudpilotInstance = instance;

            this.networkService.initialize(instance);
            this.networkService.resumeEvent.addHandler(() => this.processTimesliceNow());
        });

        this.snapshotService.snapshotRequestEvent.addHandler((cb) =>
            this.snapshotNow()
                .then(() => cb())
                .catch(cb),
        );

        const storedSession = getStoredSession();
        this.bootstrapCompletePromise = Promise.resolve();

        // We don't start with the emulator running if we were opened with an initial import
        // request -> don't recover the session in this case
        if (storedSession !== undefined && !hasInitialImportRequest()) {
            this.bootstrapCompletePromise = this.recoverStoredSession(storedSession);
        }
    }

    bootstrapComplete(): Promise<void> {
        return this.bootstrapCompletePromise;
    }

    switchSession = (id: number, options: { showLoader?: boolean } = {}): Promise<boolean> =>
        this.mutex.runExclusive(async () => {
            if (id === this.emulationContext.session()?.id) return true;

            await this.stopUnchecked();
            await this.bootstrapService.hasRendered();

            let loader: HTMLIonLoadingElement | undefined;
            const showLoader = options.showLoader ?? true;

            if (showLoader) {
                loader = await this.loadingController.create({ message: 'Loading...' });
                await loader.present();
            }

            try {
                const session = await this.storageService.getSession(id);
                if (!session) {
                    throw new Error(`invalid session ${id}`);
                }

                this.emulationContext.clearContext();

                if (!(await this.restoreSession(session))) {
                    void this.alertService.errorMessage(
                        'Failed to launch session. This may be the result of a bad ROM file.',
                    );
                    return false;
                }

                if (!this.engine) throw new Error('engine failed to initialize');

                this.emulationContext.setContext(session, this.engine);
                await this.sessionsService.updateSession({ ...session, lastLaunch: Date.now() });

                setStoredSession(id);
            } finally {
                if (loader) await loader.dismiss();
            }

            return true;
        });

    resume = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (!this.emulationContext.session() || this.errorService.hasFatalError()) {
                return;
            }
            await this.kvsService.mutex.runExclusive(() => this.updateSettings());

            await this.doResume();
        });

    pause = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            await this.doStop();

            if (!this.errorService.hasFatalError() && this.emulationContext.session()) {
                await this.snapshotNow();
            }
        });

    stop = (): Promise<void> => this.mutex.runExclusive(() => this.stopUnchecked());

    protected override updateConfiguredHotsyncName(hotsyncName: string): void {
        const session = this.emulationContext.session();
        if (!session) return;

        void this.storageService.updateSessionPartial(session.id, { hotsyncName });
    }

    protected override handleFatalInNativeCode(error: Error): void {
        this.errorService.fatalInNativeCode(error);
    }

    protected override handlePalmosStateChange(): void {
        this.isDirty = true;
    }

    protected override onAfterAdvanceEmulation(sliceSizeSeconds: number): void {
        const session = this.emulationContext.session();

        if (this.isUiInitialized() && session && session.osVersion === undefined) {
            void this.storageService.updateSessionPartial(session.id, {
                osVersion: this.getOSVersion(),
            });
        }

        this.buttonService.tick(sliceSizeSeconds);

        if (this.isDirty) {
            this.app.tick();
            this.isDirty = false;
        }
    }

    protected override handleSuspend(): void {
        if (!this.cloudpilotInstance) return;

        this.clipboardService.handleSuspend(this.cloudpilotInstance);
        this.networkService.handleSuspend();
    }

    protected override handleSnapshot(snapshot: SnapshotContainer): void {
        void this.snapshotService.storeSnapshot(snapshot);
    }

    protected override getCloudpilotInstance(): Promise<Cloudpilot> {
        return this.cloudpilotService.cloudpilot;
    }

    private async snapshotNow(): Promise<void> {
        if (!this.engine) return;

        await this.snapshotService.waitForPendingSnapshot();

        const snapshot = await this.requestSnapshot();
        if (!snapshot) {
            console.warn('failed to take snapshot');
            return;
        }

        await this.snapshotService.storeSnapshot(snapshot);
    }

    private async stopUnchecked(): Promise<void> {
        if (!this.emulationContext.session()) return;

        await this.doStop();
        await this.snapshotNow();

        this.storageCardService.onEmulatorStop();
        this.emulationContext.clearContext();
    }

    private updateSettings(): void {
        const session = this.emulationContext.session();
        if (!session) return;

        const clipboardIntegration = this.kvsService.kvs.clipboardIntegration && this.clipboardService.isSupported();

        const schedulerKind =
            this.kvsService.kvs.runHidden && this.featureService.runHidden
                ? SchedulerKind.timeout
                : SchedulerKind.animationFrame;

        const settings: Partial<EngineSettings> =
            session.engine === 'cloudpilot'
                ? {
                      speed: session.speed,
                      manageHotsyncName: session.dontManageHotsyncName,
                      hotsyncName: session.hotsyncName,
                      schedulerKind,

                      clipboardIntegration,
                      networkIntegration: this.kvsService.kvs.networkRedirection,
                  }
                : {
                      disableAudio: session.disableAudio,
                      targetMips: session.targetMips,
                      warnSlowdownThreshold: session.warnSlowdownThreshold,
                      maxHostLoad: session.maxHostLoad,
                  };

        this.updateEngineSettings(settings);
    }

    private async recoverStoredSession(session: number) {
        // This avoids a nasty delay in Safari on load. Browser bug. Juck.
        let loaderPromise: Promise<HTMLIonLoadingElement> | undefined;
        const loaderTimeout = setTimeout(() => {
            loaderPromise = this.loadingController.create({ message: 'Loading...' }).then(async (loader) => {
                await loader.present();
                return loader;
            });
        }, 100);

        try {
            await this.switchSession(session, { showLoader: false });
        } catch (e) {
            if (isIOS) {
                void this.alertService.message(
                    'Possible iOS bug',
                    `It seems that you hit an iOS bug that ocassionally
causes the database to come up empty when the app starts. If this happens to you, please force close
the app in the app switcher and reopen it; your data will be back on the second attempt.
<br/><br/>
Sorry for the inconvenience.`,
                );
            }

            console.error(e);
        } finally {
            if (loaderPromise) void loaderPromise.then((loader) => loader.dismiss());

            clearTimeout(loaderTimeout);
        }
    }

    private async restoreSession(session: Session): Promise<boolean> {
        const { rom, memory, savestate, nand } = await this.storageService.loadSession(
            session,
            this.kvsService.kvs.snapshotIntegrityCheck,
        );

        if (!rom) {
            throw new Error(`invalid ROM ${session.rom}`);
        }

        const mountedCard = session.mountedCard;
        const storageCardProvider: StorageCardProvider | undefined =
            mountedCard !== undefined
                ? {
                      load: (engine) =>
                          this.storageCardService.loadCardInEmulator(mountedCard, engine).then(() => undefined),
                  }
                : undefined;

        if (!(await this.openSession(rom, session.device, nand, memory, savestate, storageCardProvider))) {
            return false;
        }

        if (!this.engine) throw new Error('engine failed to initialize');

        this.networkService.reset();
        this.buttonService.reset(this.engine);

        await this.snapshotService.initialize(session);

        return true;
    }

    private onSessionChange = ([sessionId, session]: [number, Session | undefined]): Promise<void> =>
        this.mutex.runExclusive(async () => {
            const ctx = this.emulationContext.context();

            if (sessionId !== ctx?.session?.id) return;

            if (ctx?.session?.mountedCard !== undefined && session === undefined) {
                this.storageCardContext.release(ctx.session.mountedCard, CardOwner.cloudpilot);
            }

            if (session === undefined) {
                clearStoredSession();
                void this.stop();
            } else {
                if (ctx) this.emulationContext.setContext(session, ctx.engine);
            }
        });

    private onEmergencySave = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            const session = this.emulationContext.session();

            if (session) {
                void this.sessionsService.emergencySaveSession(session);
            }
        });

    protected override clandestineExecute: Executor = (fn) => this.ngZone.runOutsideAngular(fn);

    private mutex = new Mutex();

    private cloudpilotInstance: Cloudpilot | undefined;

    private bootstrapCompletePromise: Promise<void>;
    private isDirty = false;
}
