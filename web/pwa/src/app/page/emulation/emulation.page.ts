import { Component, ElementRef, ViewChild } from '@angular/core';
import helpUrl from '@assets/doc/emulation.md';
import { Config, LoadingController, ModalController, PopoverController } from '@ionic/angular';
import { Mutex } from 'async-mutex';

import { HelpComponent } from '@pwa/component/help/help.component';
import { debounce } from '@pwa/helper/debounce';
import { Session } from '@pwa/model/Session';
import { AlertService } from '@pwa/service/alert.service';
import { CanvasDisplayService } from '@pwa/service/canvas-display.service';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { DragDropClient, DragDropService } from '@pwa/service/drag-drop.service';
import { EmulationContextService } from '@pwa/service/emulation-context.service';
import { EmulationService } from '@pwa/service/emulation.service';
import { EventHandlingService } from '@pwa/service/event-handling.service';
import { FileService } from '@pwa/service/file.service';
import { InstallationService } from '@pwa/service/installation.service';
import { KvsService } from '@pwa/service/kvs.service';
import { LinkApi } from '@pwa/service/link-api.service';
import { NetworkService } from '@pwa/service/network.service';
import { PerformanceWatchdogService } from '@pwa/service/performance-watchdog.service';
import { SessionService } from '@pwa/service/session.service';
import { StorageService } from '@pwa/service/storage.service';
import { TabsPage } from '@pwa/tabs/tabs.page';

import { BreadcrumbMenuComponent } from './breadcrumb-menu/breadcrumb-menu.component';
import { ContextMenuComponent } from './context-menu/context-menu.component';

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
    standalone: false,
})
export class EmulationPage implements DragDropClient {
    constructor(
        public emulationService: EmulationService,
        public emulationContext: EmulationContextService,
        private storageService: StorageService,
        public eventHandlingService: EventHandlingService,
        private canvasDisplayService: CanvasDisplayService,
        private kvsService: KvsService,
        private popoverController: PopoverController,
        private modalController: ModalController,
        private alertService: AlertService,
        private fileService: FileService,
        private installlationService: InstallationService,
        public networkService: NetworkService,
        public navigation: TabsPage,
        private linkApi: LinkApi,
        public performanceWatchdogService: PerformanceWatchdogService,
        private cloudpilotService: CloudpilotService,
        private dragDropService: DragDropService,
        private sessionService: SessionService,
        private loadingController: LoadingController,
        public config: Config,
    ) {}

    get cssWidth(): string {
        return this.canvasDisplayService.width / devicePixelRatio + 'px';
    }

    get cssHeight(): string {
        return this.canvasDisplayService.height / devicePixelRatio + 'px';
    }

    ionViewWillEnter() {
        this.switching = false;
    }

    ionViewDidEnter = () =>
        this.mutex.runExclusive(async () => {
            await this.emulationService.bootstrapComplete();
            this.bootstrapComplete = true;

            const session = this.emulationContext.session();

            if (session && !session.wasResetForcefully) {
                await this.launchEmulator();
            }

            this.linkApi.installation.requestEvent.addHandler(this.handleLinkApiInstallationRequest);
            this.handleLinkApiInstallationRequest();

            this.dragDropService.registerClient(this);
        });

    ionViewWillLeave = () =>
        this.mutex.runExclusive(() => {
            this.dragDropService.unregisterClient(this);

            this.linkApi.installation.requestEvent.removeHandler(this.handleLinkApiInstallationRequest);

            if (this.emulationService.isRunning()) {
                this.autoLockUI = false;
            }
            void this.emulationService.pause();

            this.emulationService.newFrameEvent.removeHandler(this.onNewFrame);
            this.kvsService.updateEvent.removeHandler(this.onKvsUpdate);

            this.eventHandlingService.release();

            this.navigation.unlock();
        });

    @debounce()
    async openContextMenu(e: MouseEvent, reference: 'event' | 'trigger' = 'trigger'): Promise<void> {
        const popover = await this.popoverController.create({
            component: ContextMenuComponent,
            event: e,
            componentProps: {
                showHelp: () => this.showHelp(),
            },
            arrow: false,
            translucent: true,
            reference,
        });

        void popover.present();
    }

    onContextMenu(e: MouseEvent): void {
        e.preventDefault();

        void this.openContextMenu(e, 'event');
    }

    handleDragDropEvent(e: DragEvent): void | Promise<void> {
        if (!this.installFileDisabled) {
            this.fileService.openFromDrop(e, this.installlationService.installFiles.bind(this.installlationService));
        }
    }

    installFiles(): void {
        this.fileService.openFiles(this.installlationService.installFiles.bind(this.installlationService));
    }

    @debounce()
    async showHelp(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: helpUrl,
            },
        });
        await modal.present();
    }

    @debounce()
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
            },
        );
    }

    @debounce()
    async showProxyConnectedHint(): Promise<void> {
        if (this.kvsService.kvs.networkRedirectionMode === 'native') {
            await this.alertService.message('Network session', 'PalmOS has opened a network session.');
        } else {
            await this.alertService.message('Proxy connected', 'Network proxy connected.');
        }
    }

    @debounce()
    async showSlowdowndHint(): Promise<void> {
        await this.alertService.message(
            'Speed throttled',
            `
                Your host is too slow to run your virtual device at full speed, and the clock
                has been throttled in order to compensate. You can avoid this by reducing the speed of your
                virtual device in the session settings.
        `,
        );
    }

    get installFileDisabled(): boolean {
        return (
            !this.emulationService.isRunning() ||
            this.emulationService.isPowerOff() ||
            !this.emulationService.isUiInitialized()
        );
    }

    @debounce()
    async bootAfterForcefulReset(): Promise<void> {
        await this.clearForcefulReset();

        await this.launchEmulator();
    }

    @debounce()
    async bootAfterForcefulResetNoExtensions(): Promise<void> {
        await this.clearForcefulReset();

        (await this.cloudpilotService.cloudpilot).resetNoExtensions();
        await this.launchEmulator();
    }

    @debounce()
    async bootAfterForcefulResetHardReset(): Promise<void> {
        await this.clearForcefulReset();

        (await this.cloudpilotService.cloudpilot).resetHard();
        await this.launchEmulator();
    }

    get iosPlaceholderWidth(): string {
        let i = 0;

        if (this.eventHandlingService.isGameMode()) i++;
        if (this.networkService.isConnected()) i++;
        if (this.performanceWatchdogService.isSlowdownDetected()) i++;

        return i > 1 ? `${(i - 1) * 35}px` : '0';
    }

    cancelIfEmulationActive(event: TouchEvent): void {
        if (
            this.emulationContext.session() &&
            !this.emulationContext.session()?.wasResetForcefully &&
            event.cancelable
        ) {
            event.preventDefault();
        }
    }

    @debounce()
    async onBreadcrumbTrigger(trigger: string): Promise<void> {
        const menu = await this.popoverController.create({
            component: BreadcrumbMenuComponent,
            componentProps: {
                sessions: this.sessionService
                    .sessions()
                    .filter((session) => session.id !== this.emulationContext.session()?.id),
                onSelect: (session: Session) => this.switchSession(session),
            },
            dismissOnSelect: true,
            arrow: false,
            side: 'bottom',
            alignment: this.config.get('mode') === 'ios' ? 'center' : 'start',
            trigger,
            cssClass: 'cp-breadcrumb-menu',
        });

        void menu.present();
    }

    get sessions(): Array<Session> {
        return this.sessionService.sessions();
    }

    private async clearForcefulReset(): Promise<void> {
        const session = this.emulationContext.session();
        if (!session) return;

        await this.storageService.updateSessionPartial(session.id, { wasResetForcefully: false });
    }

    get title(): string {
        if (!this.bootstrapComplete) return '';
        if (this.switching) return this.lastTitle;

        return this.emulationContext.session()?.name ?? 'No session';
    }

    private async launchEmulator(): Promise<void> {
        const session = this.emulationContext.session();
        if (!session) return;

        await this.kvsService.mutex.runExclusive(
            () => this.kvsService.kvs.autoLockUI && this.autoLockUI && this.navigation.lock(),
        );

        void this.canvasDisplayService.initialize(this.canvasRef.nativeElement, session).then(() => {
            this.onKvsUpdate();
            this.kvsService.updateEvent.addHandler(this.onKvsUpdate);
        });
        this.onNewFrame(this.emulationService.getCanvas());

        this.emulationService.newFrameEvent.addHandler(this.onNewFrame);

        await this.emulationService.resume();

        this.eventHandlingService.bind(this.canvasRef.nativeElement);
    }

    private onKvsUpdate = (): void => {
        if (this.kvsService.kvs.showStatistics) {
            void this.canvasDisplayService.updateStatistics();
        } else {
            void this.canvasDisplayService.clearStatistics();
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
            void this.alertService.message(
                'Unable to install',
                `Installation is currently not possible.`,
                {},
                'Cancel',
            );
            return;
        }

        const currentSession = this.emulationContext.session();
        if (!currentSession || !this.emulationService.isRunning()) {
            void this.alertService.message(
                'Unable to install',
                `Please start a session in order to install ${url} .`,
                {},
                'Cancel',
            );
        } else {
            void this.alertService.message(
                'Installation request',
                `Do you want to install<br>${url} ?`,
                {
                    OK: () => this.fileService.openUrl(url, (file) => this.installlationService.installFiles([file])),
                },
                'Cancel',
            );
        }
    };

    private async switchSession(session: Session): Promise<void> {
        const loader = await this.loadingController.create();
        await loader.present();

        try {
            this.lastTitle = this.title;
            this.switching = true;

            await this.emulationService.pause();

            if (!(await this.emulationService.switchSession(session.id, { showLoader: false }))) {
                await this.alertService.errorMessage('Failed to launch session');
                return;
            }

            if (!session.wasResetForcefully) await this.launchEmulator();
        } finally {
            void loader.dismiss();
            this.switching = false;
        }
    }

    bootstrapComplete = false;

    switching = false;
    lastTitle = '';

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;

    private autoLockUI = true;

    private mutex = new Mutex();
}
