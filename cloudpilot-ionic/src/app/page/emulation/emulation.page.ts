import { AfterViewInit, Component, ElementRef, NgZone, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { CanvasHelper, HEIGHT, WIDTH } from './helper/CanvasHelper';
import { FileDescriptor, FileService } from 'src/app/service/file.service';
import { LoadingController, PopoverController } from '@ionic/angular';

import { AlertService } from 'src/app/service/alert.service';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EmulationService } from './../../service/emulation.service';
import { EventHandler } from './helper/EventHandler';
import { PalmButton } from '../../../../../src';

@Component({
    selector: 'app-emulation',
    templateUrl: './emulation.page.html',
    styleUrls: ['./emulation.page.scss'],
})
export class EmulationPage implements AfterViewInit {
    constructor(
        public emulationService: EmulationService,
        private popoverController: PopoverController,
        private alertService: AlertService,
        private fileService: FileService,
        private loadingController: LoadingController,
        private ngZone: NgZone
    ) {}

    ngAfterViewInit(): void {
        const canvasElt = this.canvasRef.nativeElement;
        this.canvasHelper = new CanvasHelper(canvasElt, this.emulationService);
        this.eventHandler = new EventHandler(canvasElt, this.emulationService, this.canvasHelper);

        this.canvasHelper.clear();
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

    async powerButtonDown(): Promise<void> {
        this.emulationService.handleButtonDown(PalmButton.power);
    }

    async powerButtonUp(): Promise<void> {
        this.emulationService.handleButtonUp(PalmButton.power);
    }

    async ionViewDidEnter(): Promise<void> {
        if (this.currentSession !== this.emulationService.getCurrentSession()?.id) {
            await this.canvasHelper.clear();
        }
        this.currentSession = this.emulationService.getCurrentSession()?.id;

        this.emulationService.newFrame.addHandler(this.onNewFrame);

        this.emulationService.resume();

        this.ngZone.runOutsideAngular(() => this.eventHandler.bind());
    }

    ionViewWillLeave() {
        this.emulationService.pause();

        this.emulationService.newFrame.removeHandler(this.onNewFrame);

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
            },
        });

        popover.present();
    }

    installFiles(): void {
        this.fileService.openFiles(this.processFilesForInstallation.bind(this));
    }

    get installFileDisabled(): boolean {
        return (
            !this.emulationService.isRunning() ||
            this.emulationService.isPowerOff() ||
            !this.emulationService.isUiInitialized()
        );
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

    private currentSession: number | undefined;
}
