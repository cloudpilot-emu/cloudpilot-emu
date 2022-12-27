import { ApplicationRef, Injectable, NgZone } from '@angular/core';
import { clearStoredSession, getStoredSession, setStoredSession } from '@pwa/helper/storedSession';

import { AbstractEmulationService } from '@common/service/AbstractEmulationService';
import { AlertService } from './alert.service';
import { BootstrapService } from './bootstrap-service';
import { ButtonService } from './button.service';
import { ClipboardService } from './clipboard.service';
import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { CloudpilotService } from './cloudpilot.service';
import { EmulationStateService } from './emulation-state.service';
import { ErrorService } from './error.service';
import { FileService } from './file.service';
import { KvsService } from './kvs.service';
import { LoadingController } from '@ionic/angular';
import { ModalWatcherService } from './modal-watcher.service';
import { Mutex } from 'async-mutex';
import { ProxyService } from './proxy.service';
import { SchedulerKind } from '@common/helper/scheduler';
import { Session } from '@pwa/model/Session';
import { SnapshotService } from './snapshot.service';
import { StorageCardService } from '@pwa/service/storage-card.service';
import { StorageService } from './storage.service';
import { hasInitialImportRequest } from './link-api.service';
import { isIOS } from '@common/helper/browser';

const SNAPSHOT_INTERVAL = 1000;

@Injectable({ providedIn: 'root' })
export class EmulationService extends AbstractEmulationService {
    constructor(
        private storageService: StorageService,
        private ngZone: NgZone,
        private loadingController: LoadingController,
        private emulationState: EmulationStateService,
        private snapshotService: SnapshotService,
        private errorService: ErrorService,
        private fileService: FileService,
        private alertService: AlertService,
        private modalWatcher: ModalWatcherService,
        private clipboardService: ClipboardService,
        private kvsService: KvsService,
        private proxyService: ProxyService,
        private buttonService: ButtonService,
        private bootstrapService: BootstrapService,
        private cloudpilotService: CloudpilotService,
        private storageCardService: StorageCardService,
        private app: ApplicationRef
    ) {
        super();

        storageService.sessionChangeEvent.addHandler(this.onSessionChange);
        errorService.fatalErrorEvent.addHandler(this.pause);
        this.alertService.emergencySaveEvent.addHandler(this.onEmergencySave);

        this.cloudpilotService.cloudpilot.then((instance) => {
            instance.fatalErrorEvent.addHandler(this.errorService.fatalInNativeCode);

            this.proxyService.initialize(instance);
            this.proxyService.resumeEvent.addHandler(() => this.running && this.advanceEmulation(performance.now()));
        });

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

    switchSession = (id: number): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (id === this.emulationState.getCurrentSession()?.id) {
                return;
            }

            await this.bootstrapService.hasRendered();
            const loader = await this.loadingController.create({ message: 'Loading...' });
            await loader.present();

            try {
                const session = await this.storageService.getSession(id);
                this.emulationState.setCurrentSession(session);

                if (!session) {
                    throw new Error(`invalid session ${id}`);
                }

                const cloudpilot = await this.cloudpilotService.cloudpilot;

                await this.restoreSession(session, cloudpilot);

                setStoredSession(id);
            } finally {
                await loader.dismiss();
            }
        });

    resume = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (
                !this.emulationState.getCurrentSession() ||
                this.running ||
                this.errorService.hasFatalError() ||
                !this.cloudpilotInstance
            ) {
                return;
            }
            await this.kvsService.mutex.runExclusive(() => this.updateFeatures());

            this.doResume();

            this.lastSnapshotAt = performance.now();
        });

    pause = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            this.doPause();

            if (!this.errorService.hasFatalError() && this.emulationState.getCurrentSession()) {
                await this.snapshotService.waitForPendingSnapshot();
                await this.snapshotService.triggerSnapshot();
            }
        });

    stop = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            this.doStop();

            this.emulationState.setCurrentSession(undefined);
        });

    protected getConfiguredSpeed(): number {
        return this.emulationState.getCurrentSession()?.speed || 1;
    }

    protected manageHotsyncName(): boolean {
        return !this.emulationState.getCurrentSession()?.dontManageHotsyncName;
    }

    protected getConfiguredHotsyncName(): string | undefined {
        return this.emulationState.getCurrentSession()?.hotsyncName;
    }

    protected updateConfiguredHotsyncName(hotsyncName: string): void {
        const session = this.emulationState.getCurrentSession();
        if (!session) return;

        this.storageService.updateSessionPartial(session.id, { hotsyncName });
    }

    protected getConfiguredSchdedulerKind(): SchedulerKind {
        return this.kvsService.kvs.runHidden ? SchedulerKind.timeout : SchedulerKind.animationFrame;
    }

    protected hasFatalError(): boolean {
        return this.errorService.hasFatalError();
    }

    protected skipEmulationUpdate(): boolean {
        return this.modalWatcher.isModalActive() && this.frameCounter !== 0;
    }

    protected override onAfterAdvanceEmulation(timestamp: number, cycles: number): void {
        if (!this.cloudpilotInstance) return;

        const session = this.emulationState.getCurrentSession();

        if (this.uiInitialized && session && session.osVersion === undefined) {
            this.storageService.updateSessionPartial(session.id, { osVersion: this.cloudpilotInstance.getOSVersion() });
        }

        this.buttonService.tick(cycles / this.cloudpilotInstance.cyclesPerSecond());

        if (
            timestamp - this.lastSnapshotAt > SNAPSHOT_INTERVAL &&
            this.isUiInitialized() &&
            !this.cloudpilotInstance.isSuspended()
        ) {
            this.snapshotService.triggerSnapshot();

            this.lastSnapshotAt = timestamp;
        }

        if (this.isDirty) this.app.tick();
    }

    protected override handleSuspend(): void {
        if (!this.cloudpilotInstance) return;

        this.clipboardService.handleSuspend(this.cloudpilotInstance);
        this.proxyService.handleSuspend();
    }

    protected override callScheduler(): void {
        this.ngZone.runOutsideAngular(() => this.scheduler.schedule());
    }

    private updateFeatures(): void {
        if (!this.cloudpilotInstance) return;

        const clipboardIntegrationEnabled =
            this.kvsService.kvs.clipboardIntegration && this.clipboardService.isSupported();

        if (this.cloudpilotInstance.getClipboardIntegration() !== clipboardIntegrationEnabled) {
            this.cloudpilotInstance.setClipboardIntegration(clipboardIntegrationEnabled);
        }

        if (this.cloudpilotInstance.getNetworkRedirection() !== this.kvsService.kvs.networkRedirection) {
            this.cloudpilotInstance.setNetworkRedirection(this.kvsService.kvs.networkRedirection);

            this.proxyService.reset();
        }

        this.cloudpilotInstance.setHotsyncNameManagement(
            !this.emulationState.getCurrentSession()?.dontManageHotsyncName
        );
    }

    private async recoverStoredSession(session: number) {
        try {
            await this.switchSession(session);
        } catch (e) {
            if (isIOS) {
                this.alertService.message(
                    'Possible iOS bug',
                    `It seems that you hit an iOS bug that ocassionally
causes the database to come up empty when the app starts. If this happens to you, please force close
the app in the app switcher and reopen it; your data will be back on the second attempt.
<br/><br/>
Sorry for the inconvenience.`
                );
            }
        }
    }

    private async restoreSession(session: Session, cloudpilot: Cloudpilot): Promise<void> {
        const [rom, memory, state] = await this.storageService.loadSession(session);
        if (!rom) {
            throw new Error(`invalid ROM ${session.rom}`);
        }

        cloudpilot.clearExternalStorage();

        if (session.mountedCard !== undefined) {
            await this.storageCardService.loadCard(session.mountedCard);
        }

        if (!this.openSession(cloudpilot, rom, session.device, memory, state)) {
            throw new Error('failed to open session');
        }

        this.proxyService.reset();
        this.buttonService.reset(cloudpilot);

        await this.snapshotService.initialize(session, await this.cloudpilotService.cloudpilot);
    }

    private onSessionChange = ([sessionId, session]: [number, Session | undefined]): Promise<void> =>
        this.mutex.runExclusive(async () => {
            if (sessionId !== this.emulationState.getCurrentSession()?.id) return;

            this.emulationState.setCurrentSession(session);
            if (!this.emulationState.getCurrentSession()) {
                clearStoredSession();

                this.stopLoop();
            }
        });

    private onEmergencySave = (): Promise<void> =>
        this.mutex.runExclusive(async () => {
            const session = this.emulationState.getCurrentSession();

            if (session) {
                this.fileService.emergencySaveSession(session, await this.cloudpilotService.cloudpilot);
            }
        });

    protected mutex = new Mutex();
    private bootstrapCompletePromise: Promise<void>;
    private lastSnapshotAt = 0;
}
