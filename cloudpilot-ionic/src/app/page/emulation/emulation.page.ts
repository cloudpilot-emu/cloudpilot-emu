import { AfterViewInit, Component, ElementRef, ViewChild } from '@angular/core';
import { ModalController, PopoverController } from '@ionic/angular';

import { AlertService } from 'src/app/service/alert.service';
import { AudioService } from './../../service/audio.service';
import { CanvasDisplayService } from './../../service/canvas-display.service';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EmulationService } from './../../service/emulation.service';
import { EmulationStateService } from './../../service/emulation-state.service';
import { EventHandlingService } from './../../service/event-handling.service';
import { FileService } from 'src/app/service/file.service';
import { HelpComponent } from 'src/app/component/help/help.component';
import { InstallationService } from './../../service/installation.service';
import { KvsService } from './../../service/kvs.service';
import { SnapshotService } from './../../service/snapshot.service';
import { SnapshotStatistics } from './../../model/SnapshotStatistics';
import { StorageService } from './../../service/storage.service';
import { getStoredSession } from 'src/app/helper/storedSession';

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage implements AfterViewInit {
    constructor(
        public emulationService: EmulationService,
        public emulationState: EmulationStateService,
        private storageService: StorageService,
        public eventHandlingService: EventHandlingService,
        private canvasDisplayService: CanvasDisplayService,
        private kvsService: KvsService,
        private audioService: AudioService,
        private popoverController: PopoverController,
        private modalController: ModalController,
        private alertService: AlertService,
        private fileService: FileService,
        private snapshotService: SnapshotService,
        private installlationService: InstallationService
    ) {}

    ngAfterViewInit(): void {}

    get cssWidth(): string {
        return this.canvasDisplayService.width / devicePixelRatio + 'px';
    }

    get cssHeight(): string {
        return this.canvasDisplayService.height / devicePixelRatio + 'px';
    }

    async ionViewDidEnter(): Promise<void> {
        let session = this.emulationState.getCurrentSession();
        const storedSession = getStoredSession();

        if (!session && storedSession !== undefined) {
            session = await this.storageService.getSession(storedSession);
        }
        this.canvasDisplayService.initialize(this.canvasRef.nativeElement, session).then(() => {
            if (this.kvsService.kvs.showStatistics) this.canvasDisplayService.drawStatistics();
        });
        this.onNewFrame(this.emulationService.getCanvas());

        this.emulationService.newFrameEvent.addHandler(this.onNewFrame);
        this.snapshotService.snapshotEvent.addHandler(this.onSnapshot);

        this.emulationService.resume();

        this.eventHandlingService.bind(this.canvasRef.nativeElement);
    }

    ionViewWillLeave() {
        this.emulationService.pause();

        this.emulationService.newFrameEvent.removeHandler(this.onNewFrame);
        this.snapshotService.snapshotEvent.removeHandler(this.onSnapshot);

        this.eventHandlingService.release();
    }

    async openContextMenu(e: MouseEvent): Promise<void> {
        const popover = await this.popoverController.create({
            component: ContextMenuComponent,
            event: e,
            backdropDismiss: true,
            showBackdrop: false,
            componentProps: {
                onClick: () => popover.dismiss(),
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

    get isMuted(): boolean {
        return !this.audioService.isInitialized() || this.audioService.isMuted();
    }

    get isAudioOff(): boolean {
        return this.kvsService.kvs.volume <= 0;
    }

    mute(muted: boolean): void {
        if (muted) {
            this.audioService.mute(true);
        } else {
            if (this.audioService.isInitialized()) {
                this.audioService.mute(false);
            } else {
                this.audioService.initialize();
            }
        }
    }

    private onSnapshot = (statistics: SnapshotStatistics): void => {
        if (this.kvsService.kvs.showStatistics) {
            this.canvasDisplayService.drawStatistics(statistics, this.emulationService.getStatistics());
        }
    };

    private onNewFrame = (canvas: HTMLCanvasElement): void => {
        this.canvasDisplayService.drawEmulationCanvas(canvas);
    };

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;
}
