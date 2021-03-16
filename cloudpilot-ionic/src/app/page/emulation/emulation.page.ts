import { AfterViewInit, Component, ElementRef, NgZone, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { CanvasHelper, HEIGHT, WIDTH } from './helper/CanvasHelper';
import { FileDescriptor, FileService } from 'src/app/service/file.service';
import { LoadingController, ModalController, PopoverController } from '@ionic/angular';
import { getStoredSession, hasStoredSession } from 'src/app/helper/storedSession';

import { AlertService } from 'src/app/service/alert.service';
import { AudioService } from './../../service/audio.service';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EmulationService } from './../../service/emulation.service';
import { EmulationStateService } from './../../service/emulation-state.service';
import { EventHandler } from './helper/EventHandler';
import { HelpComponent } from 'src/app/component/help/help.component';
import { KvsService } from './../../service/kvs.service';
import { PalmButton } from '../../../../../src';
import { StorageService } from './../../service/storage.service';

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
        private kvsService: KvsService,
        private audioService: AudioService,
        private popoverController: PopoverController,
        private modalController: ModalController,
        private alertService: AlertService,
        private fileService: FileService,
        private loadingController: LoadingController,
        private ngZone: NgZone
    ) {}

    ngAfterViewInit(): void {
        const canvasElt = this.canvasRef.nativeElement;
        this.canvasHelper = new CanvasHelper(canvasElt);
        this.eventHandler = new EventHandler(canvasElt, this.emulationService, this.canvasHelper);
    }

    get canvasWidth(): number {
        return WIDTH;
    }

    get canvasHeight(): number {
        return HEIGHT;
    }

    get cssWidth(): string {
        return this.canvasWidth / devicePixelRatio + 'px';
    }

    get cssHeight(): string {
        return this.canvasHeight / devicePixelRatio + 'px';
    }

    async ionViewDidEnter(): Promise<void> {
        let session = this.emulationState.getCurrentSession();
        const storedSession = getStoredSession();

        if (!session && storedSession !== undefined) {
            session = await this.storageService.getSession(storedSession);
        }
        this.canvasHelper.clear(session);
        this.onNewFrame(this.emulationService.getCanvas());

        this.emulationService.newFrameEvent.addHandler(this.onNewFrame);

        this.emulationService.resume();

        this.ngZone.runOutsideAngular(() => this.eventHandler.bind());
    }

    ionViewWillLeave() {
        this.emulationService.pause();

        this.emulationService.newFrameEvent.removeHandler(this.onNewFrame);

        this.eventHandler.release();
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
        this.fileService.openFiles(this.processFilesForInstallation.bind(this));
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

    private onNewFrame = (canvas: HTMLCanvasElement): void => {
        this.canvasHelper.drawEmulationCanvas(canvas);
    };

    private async processFilesForInstallation(files: Array<FileDescriptor>): Promise<void> {
        const loader = await this.loadingController.create({
            message: 'Installing...',
        });
        await loader.present();

        const filesSuccess: Array<string> = [];
        const filesFail: Array<string> = [];

        try {
            for (const file of files) {
                if (!/\.(prc|pdb)$/i.test(file.name)) {
                    filesFail.push(file.name);

                    continue;
                }

                if ((await this.emulationService.installFile(file.content)) == 0) {
                    filesSuccess.push(file.name);
                } else {
                    filesFail.push(file.name);
                }
            }
        } finally {
            loader.dismiss();
        }

        let message: string;

        switch (filesSuccess.length) {
            case 0:
                message = '';
                break;

            case 1:
                message = `Installation of ${filesSuccess[0]} successful.`;
                break;

            default:
                message = `Installation of ${filesSuccess.length} files successful.`;
                break;
        }

        if (filesFail.length > 0) message += '<br/><br/>';

        switch (filesFail.length) {
            case 0:
                break;

            case 1:
                message += `Installation of ${filesFail[0]} failed.`;
                break;

            default:
                message += `Installation of ${filesFail
                    .slice(0, filesFail.length > 3 ? 3 : filesFail.length - 1)
                    .join(', ')} and ${
                    filesFail.length > 3 ? filesFail.length - 3 + ' more files' : filesFail[filesFail.length - 1]
                } failed.`;

                break;
        }

        let header;

        if (filesFail.length === 0) {
            header = 'Installation successful';
        } else if (filesSuccess.length === 0) {
            header = 'Installation failed';
        } else {
            header = 'Installation errors';
        }

        this.alertService.message(header, message);
    }

    @ViewChild('canvas') private canvasRef!: ElementRef<HTMLCanvasElement>;
    private canvasHelper!: CanvasHelper;
    private eventHandler!: EventHandler;
}
