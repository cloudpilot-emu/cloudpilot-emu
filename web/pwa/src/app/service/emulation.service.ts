import { Inject, Injectable, NgZone } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { StorageCardProvider } from '@common/engine/Engine';
import { EngineSettings } from '@common/engine/EngineSettings';
import { SnapshotContainer } from '@common/engine/Snapshot';
import { LoaderHandle } from '@common/helper/Loader';
import { isIOS } from '@common/helper/browser';
import { SchedulerKind } from '@common/helper/scheduler';
import { AbstractEmulationService, Executor } from '@common/service/AbstractEmulationService';
import { Mutex } from 'async-mutex';
import { environment } from 'pwa/src/environments/environment';

import { Lock } from '@pwa/helper/lock';
import { clearStoredSession, getStoredSession, setStoredSession } from '@pwa/helper/storedSession';
import { Session } from '@pwa/model/Session';
import { StorageCardService } from '@pwa/service/storage-card.service';

import { AlertService } from './alert.service';
import { BootstrapService } from './bootstrap-service';
import { ButtonService } from './button.service';
import { ClipboardService } from './clipboard.service';
import { EmulationContextService } from './emulation-context.service';
import { ErrorService } from './error.service';
import { FeatureService } from './feature.service';
import { KvsService } from './kvs.service';
import { hasInitialImportRequest } from './link-api.service';
import { LoaderService } from './loader.service';
import { ModalWatcherService } from './modal-watcher.service';
import { NetworkService } from './network.service';
import { SessionService } from './session.service';
import { SnapshotService } from './snapshot.service';
import { CardOwner, StorageCardContext } from './storage-card-context';
import { StorageService } from './storage.service';
import { TOKEN_CLOUDPILOT_INSTANCE, TOKEN_EMULATOR_LOCK, TOKEN_UARM_MODULE } from './token';

// TODO: Get rid of cloudpilot service

const SNAPSHOT_INTERVAL = 1000;

@Injectable({ providedIn: 'root' })
export class EmulationService extends AbstractEmulationService {
    constructor(
        private storageService: StorageService,
        private ngZone: NgZone,
        private loaderService: LoaderService,
        private emulationContext: EmulationContextService,
        private snapshotService: SnapshotService,
        private errorService: ErrorService,
        private alertService: AlertService,
        private clipboardService: ClipboardService,
        private kvsService: KvsService,
        private networkService: NetworkService,
        private buttonService: ButtonService,
        private bootstrapService: BootstrapService,
        private storageCardService: StorageCardService,
        private storageCardContext: StorageCardContext,
        private sessionsService: SessionService,
        private featureService: FeatureService,
        modalWatcher: ModalWatcherService,
        @Inject(TOKEN_CLOUDPILOT_INSTANCE) cloudpilotPromise: Promise<Cloudpilot>,
        @Inject(TOKEN_UARM_MODULE) private uarmModulePromise: Promise<WebAssembly.Module>,
        @Inject(TOKEN_EMULATOR_LOCK) emulatorLock: Lock,
    ) {
        super(cloudpilotPromise);

        storageService.sessionChangeEvent.addHandler(this.onSessionChange);
        errorService.fatalErrorEvent.addHandler(this.pause);
        this.alertService.emergencySaveEvent.addHandler(this.onEmergencySave);

        void cloudpilotPromise.then((instance) => {
            this.cloudpilotInstance = instance;

            this.networkService.initialize(instance);
            this.networkService.resumeEvent.addHandler(() => this.processTimesliceNow());
        });

        this.snapshotService.snapshotRequestEvent.addHandler((cb) =>
            this.mutex
                .runExclusive(() => this.snapshotNow())
                .then(() => cb())
                .catch(cb),
        );

        emulatorLock.lockEvent.addHandler(() => void this.block());
        emulatorLock.unlockEvent.addHandler(() => void this.unblock());

        modalWatcher.modalVisibilityChangeEvent.addHandler((isVisible) =>
            isVisible ? void emulatorLock.lock() : void emulatorLock.unlock(),
        );

        this.newFrameEvent.addHandler(() => modalWatcher.enable());

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
            if (this.errorService.hasFatalError()) return false;
            if (id === this.emulationContext.session()?.id) return true;

            await this.stopUnchecked();
            await this.bootstrapService.hasRendered();

            let loaderHandle: LoaderHandle | undefined;
            const showLoader = options.showLoader ?? true;

            if (showLoader) {
                loaderHandle = await this.loaderService.add('Loading...');
            }

            try {
                const session = await this.storageService.getSession(id);
                if (!session) {
                    throw new Error(`invalid session ${id}`);
                }

                this.emulationContext.clearContext();

                if (!(await this.restoreSession(session))) return false;

                if (!this.engine) throw new Error('engine failed to initialize');

                this.emulationContext.setContext(session, this.engine);
                await this.sessionsService.updateSession({ ...session, lastLaunch: Date.now() });

                setStoredSession(id);
            } finally {
                if (loaderHandle !== undefined) this.loaderService.resolve(loaderHandle);
            }

            return true;
        });

    resume = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (this.errorService.hasFatalError()) return;

            await this.kvsService.mutex.runExclusive(() => this.updateSettings());
            await this.doResume();
        });

    pause = (): Promise<void> => this.mutex.runExclusive(() => this.doStop());

    stop = (): Promise<void> => this.mutex.runExclusive(() => this.stopUnchecked());

    async waitForPendingSnapshot(): Promise<void> {
        return this.engine?.waitForPendingSnapshot();
    }

    flush(): Promise<void> {
        return this.mutex.waitForUnlock();
    }

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
        const osVersion = this.getOSVersion();

        if (this.isUiInitialized() && session && session.osVersion !== osVersion) {
            void this.storageService.updateSessionPartial(session.id, {
                osVersion,
            });
        }

        this.buttonService.tick(sliceSizeSeconds);

        if (this.isDirty) {
            this.ngZone.run(() => undefined);
            this.isDirty = false;
        }
    }

    protected override handleSuspend(): void {
        if (!this.cloudpilotInstance) return;

        this.clipboardService.handleSuspend(this.cloudpilotInstance);
        this.networkService.handleSuspend();
    }

    protected override handleSnapshot(snapshot: SnapshotContainer): void {
        const session = this.emulationContext.session();
        if (!session) throw new Error('unreachable: handleSnapshot without session');

        void this.snapshotService.storeSnapshot(snapshot, session.id);
    }

    protected override getUarmModule(): Promise<WebAssembly.Module> {
        return this.uarmModulePromise;
    }

    private block = (): Promise<void> => this.mutex.runExclusive(() => this.doBlock());

    private unblock = (): Promise<void> => this.mutex.runExclusive(() => this.doUnblock());

    private async snapshotNow(): Promise<void> {
        if (!this.engine) return;

        if (this.isRunning()) {
            const error = new Error('Immediate snapshot request while emulator is running');
            this.errorService.fatalBug(error.message);

            throw error;
        }

        const session = this.emulationContext.session();
        if (!session) throw new Error(`unreachable: emulator running w/o session`);

        await this.engine.waitForPendingSnapshot();

        const snapshot = await this.requestSnapshot();
        if (!snapshot) {
            console.warn('failed to take snapshot');
            return;
        }

        await this.snapshotService.storeSnapshot(snapshot, session.id);
        await this.engine.waitForPendingSnapshot();
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
                      manageHotsyncName: !session.dontManageHotsyncName,
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
                      runInBackground: this.kvsService.kvs.runHidden,
                  };

        if (settings.networkIntegration !== this.engineSettings.networkIntegration) this.networkService.reset();

        this.updateEngineSettings({
            ...settings,
            memoryCrc: this.kvsService.kvs.snapshotIntegrityCheck && environment.debug,
        });
    }

    private async recoverStoredSession(session: number) {
        try {
            // The delay avoids a nasty delay in Safari on load. Browser bug. Juck.
            await this.loaderService.showWhile(
                () => this.switchSession(session, { showLoader: false }),
                'Loading...',
                1,
            );
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

        const storageCard =
            session.mountedCard !== undefined ? await this.storageService.getCard(session.mountedCard) : undefined;

        const storageCardProvider: StorageCardProvider | undefined =
            storageCard !== undefined
                ? {
                      storageKey: storageCard?.storageId,
                      load: (engine) =>
                          this.storageCardService.loadCardInEmulator(storageCard.id, engine).then(() => undefined),
                  }
                : undefined;

        if (!(await this.openSession(rom, session.device, nand, memory, savestate, storageCardProvider))) {
            return false;
        }

        if (!this.engine) throw new Error('engine failed to initialize');

        this.networkService.reset();
        this.buttonService.reset(this.engine);

        this.updateEngineSettings({ automaticSnapshotInterval: SNAPSHOT_INTERVAL });
        await this.snapshotService.initialize(session);

        return true;
    }

    private onSessionChange = ([sessionId, session]: [number, Session | undefined]): void => {
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
            this.updateSettings();
        }
    };

    private onEmergencySave = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            const session = this.emulationContext.session();
            if (!session || !this.engine) return;

            const fullState = await this.engine.getFullState();

            if (fullState) {
                void this.sessionsService.emergencySaveSession(
                    session,
                    fullState.rom,
                    fullState.memory,
                    fullState.nand,
                    fullState.savestate,
                );
            }
        });

    protected override clandestineExecute: Executor = (fn) => this.ngZone.runOutsideAngular(fn);

    private mutex = new Mutex();

    private cloudpilotInstance: Cloudpilot | undefined;

    private bootstrapCompletePromise: Promise<void>;
    private isDirty = false;
}
