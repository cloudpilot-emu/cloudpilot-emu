import { Component, ElementRef, ViewChild } from '@angular/core';
import { ModalController, PopoverController } from '@ionic/angular';

import { AlertService } from '@pwa/service/alert.service';
import { CanvasDisplayService } from '@pwa/service/canvas-display.service';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EmulationService } from '@pwa/service/emulation.service';
import { EmulationStateService } from '@pwa/service/emulation-state.service';
import { EventHandlingService } from '@pwa/service/event-handling.service';
import { FileService } from '@pwa/service/file.service';
import { HelpComponent } from '@pwa/component/help/help.component';
import { InstallationService } from '@pwa/service/installation.service';
import { KvsService } from '@pwa/service/kvs.service';
import { LinkApi } from '@pwa/service/link-api.service';
import { Mutex } from 'async-mutex';
import { PerformanceWatchdogService } from '@pwa/service/performance-watchdog.service';
import { ProxyService } from '@pwa/service/proxy.service';
import { SnapshotService } from '@pwa/service/snapshot.service';
import { SnapshotStatistics } from '@pwa/model/SnapshotStatistics';
import { StorageService } from '@pwa/service/storage.service';
import { TabsPage } from '@pwa/tabs/tabs.page';

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage {
    constructor(
        public emulationService: EmulationService,
        public emulationState: EmulationStateService,
        private storageService: StorageService,
        public eventHandlingService: EventHandlingService,
        private canvasDisplayService: CanvasDisplayService,
        private kvsService: KvsService,
        private popoverController: PopoverController,
        private modalController: ModalController,
        private alertService: AlertService,
        private fileService: FileService,
        private snapshotService: SnapshotService,
        private installlationService: InstallationService,
        public proxyService: ProxyService,
        public navigation: TabsPage,
        private linkApi: LinkApi,
        public performanceWatchdogService: PerformanceWatchdogService,
        private cloudpilotService: CloudpilotService
    ) {}

    get cssWidth(): string {
        return this.canvasDisplayService.width / devicePixelRatio + 'px';
    }

    get cssHeight(): string {
        return this.canvasDisplayService.height / devicePixelRatio + 'px';
    }

    ionViewDidEnter = () =>
        this.mutex.runExclusive(async () => {
            await this.emulationService.bootstrapComplete();
            this.boostrapComplete = true;

            const session = this.emulationState.getCurrentSession();

            if (session && !session.wasResetForcefully) {
                await this.launchEmulator();
            }

            this.linkApi.installation.requestEvent.addHandler(this.handleLinkApiInstallationRequest);
            this.handleLinkApiInstallationRequest();
        });

    ionViewWillLeave = () =>
        this.mutex.runExclusive(() => {
            this.linkApi.installation.requestEvent.removeHandler(this.handleLinkApiInstallationRequest);

            if (this.emulationService.isRunning()) {
                this.autoLockUI = false;
            }
            this.emulationService.pause();

            this.emulationService.newFrameEvent.removeHandler(this.onNewFrame);
            this.snapshotService.snapshotEvent.removeHandler(this.onSnapshot);
            this.kvsService.updateEvent.removeHandler(this.onKvsUpdate);

            this.eventHandlingService.release();

            this.navigation.unlock();
        });

    async openContextMenu(e: MouseEvent): Promise<void> {
        const popover = await this.popoverController.create({
            component: ContextMenuComponent,
            event: e,
            backdropDismiss: true,
            showBackdrop: false,
            componentProps: {
                showHelp: () => this.showHelp(),
            },
        });

        popover.present();
    }

    installFiles(): void {
        this.fileService.openFiles(this.installlationService.installFiles.bind(this.installlationService));
    }

    async showHelp(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: 'assets/doc/emulation.md',
            },
        });
        await modal.present();
    }

    async showGameModeHint(): Promise<void> {
        await this.alertService.message(
            'Game mode',
            `
                The keyboard is in game mode and controls the hardware buttons.
                Press ctrl-shift in order to exit game mode.
                <br/><br/>
                Check the help page for a description of the different keyboard mappings.
            `,
            {
                Help: () => this.showHelp(),
            }
        );
    }

    async showProxyConnectedHint(): Promise<void> {
        await this.alertService.message('Proxy connected', 'Network proxy connected.');
    }

    async showSlowdowndHint(): Promise<void> {
        await this.alertService.message(
            'Speed throttled',
            `
                Your host is too slow to run your virtual device at full speed, and the clock
                has been throttled in order to compensate. You can avoid this by reducing the speed of your
                virtual device in the session settings.
        `
        );
    }

    get installFileDisabled(): boolean {
        return (
            !this.emulationService.isRunning() ||
            this.emulationService.isPowerOff() ||
            !this.emulationService.isUiInitialized()
        );
    }

    get title(): string {
        return this.emulationState.getCurrentSession()?.name || '';
    }

    async bootAfterForcefulReset(): Promise<void> {
        await this.clearForcefulReset();

        await this.launchEmulator();
    }

    async bootAfterForcefulResetNoExtensions(): Promise<void> {
        await this.clearForcefulReset();

        (await this.cloudpilotService.cloudpilot).resetNoExtensions();
        await this.launchEmulator();
    }

    async bootAfterForcefulResetHardReset(): Promise<void> {
        await this.clearForcefulReset();

        (await this.cloudpilotService.cloudpilot).resetHard();
        await this.launchEmulator();
    }

    private async clearForcefulReset(): Promise<void> {
        const session = this.emulationState.getCurrentSession();
        if (!session) return;

        session.wasResetForcefully = false;
        await this.storageService.updateSession(session);
    }

    private async launchEmulator(): Promise<void> {
        const session = this.emulationState.getCurrentSession();
        if (!session) return;

        await this.kvsService.mutex.runExclusive(
            () => this.kvsService.kvs.autoLockUI && this.autoLockUI && this.navigation.lock()
        );

        this.canvasDisplayService.initialize(this.canvasRef.nativeElement, session).then(() => {
            this.onKvsUpdate();
            this.kvsService.updateEvent.addHandler(this.onKvsUpdate);
        });
        this.onNewFrame(this.emulationService.getCanvas());

        this.emulationService.newFrameEvent.addHandler(this.onNewFrame);
        this.snapshotService.snapshotEvent.addHandler(this.onSnapshot);

        await this.emulationService.resume();

        this.eventHandlingService.bind(this.canvasRef.nativeElement);
    }

    private onKvsUpdate = async (): Promise<void> => {
        if (this.kvsService.kvs.showStatistics) {
            this.canvasDisplayService.updateStatistics();
        } else {
            this.canvasDisplayService.clearStatistics();
        }
    };

    private onSnapshot = (statistics: SnapshotStatistics): void => {
        if (this.kvsService.kvs.showStatistics) {
            this.canvasDisplayService.updateStatistics(statistics, this.emulationService.getStatistics());
        }
    };

    private onNewFrame = (canvas: HTMLCanvasElement): void => {
        this.canvasDisplayService.updateEmulationCanvas(canvas);
    };

    private handleLinkApiInstallationRequest = (): void => {
        if (!this.linkApi.installation.hasPendingRequest()) {
            return;
        }

        const url = this.linkApi.installation.receivePendingUrl();
        if (!url) {
            return;
        }

        if (this.installFileDisabled) {
            this.alertService.message('Unable to install', `Installation is currently not possible.`, {}, 'Cancel');
            return;
        }

        const currentSession = this.emulationState.getCurrentSession();
        if (!currentSession || !this.emulationService.isRunning()) {
            this.alertService.message(
                'Unable to install',
                `Please start a session in order to install ${url} .`,
                {},
                'Cancel'
            );
        } else {
            this.alertService.message(
                'Installation request',
                `Do you want to install<br>${url} ?`,
                {
                    OK: () => this.fileService.openUrl(url, (file) => this.installlationService.installFiles([file])),
                },
                'Cancel'
            );
        }
    };

    boostrapComplete = false;
    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;

    private autoLockUI = true;

    private mutex = new Mutex();
}
