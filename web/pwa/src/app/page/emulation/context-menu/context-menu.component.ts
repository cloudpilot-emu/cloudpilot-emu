import { ActionSheetController, ModalController, PopoverController } from '@ionic/angular';
import { Component, Input, OnInit } from '@angular/core';
import { quirkNoPoweroff, supportsDBExport } from '@common/helper/deviceProperties';

import { AudioService } from './../../../service/audio.service';
import { BackupService } from './../../../service/backup.service';
import { ButtonService } from '../../../service/button.service';
import { CanvasDisplayService } from './../../../service/canvas-display.service';
import { DeviceOrientation } from '../../../../../../common/model/DeviceOrientation';
import { EmulationService } from './../../../service/emulation.service';
import { EmulationStateService } from '../../../service/emulation-state.service';
import { KvsService } from './../../../service/kvs.service';
import { PalmButton } from '../../../../../../../src';
import { PerformanceWatchdogService } from '../../../service/performance-watchdog.service';
import { SessionService } from '../../../service/session.service';
import { SessionSettingsComponent } from '../../../component/session-settings/session-settings.component';

@Component({
    selector: 'app-emulation-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
})
export class ContextMenuComponent implements OnInit {
    constructor(
        public emulationService: EmulationService,
        private emulationStateService: EmulationStateService,
        private backupService: BackupService,
        private buttonService: ButtonService,
        private actionSheetController: ActionSheetController,
        private popoverController: PopoverController,
        private audioService: AudioService,
        private kvsService: KvsService,
        private modalController: ModalController,
        private sessionService: SessionService,
        private performanceWatchdogService: PerformanceWatchdogService,
        private canvasDisplayService: CanvasDisplayService
    ) {}

    ngOnInit(): void {}

    async reset(): Promise<void> {
        this.popoverController.dismiss();

        const sheet = await this.actionSheetController.create({
            header: 'How do you want to reset?',
            buttons: [
                {
                    text: 'Normal boot',
                    handler: () => this.emulationService.reset(),
                },
                {
                    text: 'No extensions',
                    handler: () => this.emulationService.resetNoExtensions(),
                },
                {
                    text: 'Hard reset',
                    handler: () => this.emulationService.resetHard(),
                },
                {
                    text: 'Cancel',
                    role: 'cancel',
                },
            ],
        });

        sheet.present();
    }

    power(): void {
        this.buttonService.engage(PalmButton.power);

        this.popoverController.dismiss();
    }

    hotsync(): void {
        this.buttonService.engage(PalmButton.cradle);

        this.popoverController.dismiss();
    }

    help(): void {
        this.showHelp();

        this.popoverController.dismiss();
    }

    async backup(): Promise<void> {
        this.popoverController.dismiss();

        const sheet = await this.actionSheetController.create({
            header: 'What do you want to backup?',
            buttons: [
                {
                    text: 'Only RAM (default)',
                    handler: () => this.backupService.saveBackup(false),
                },
                {
                    text: 'Include ROM',
                    handler: () => this.backupService.saveBackup(true),
                },
                {
                    text: 'Cancel',
                    role: 'cancel',
                },
            ],
        });

        sheet.present();
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

        this.popoverController.dismiss();
    }

    get backupDisabled(): boolean {
        const currentSession = this.emulationStateService.getCurrentSession();

        return (
            !this.emulationService.isRunning() ||
            this.emulationService.isPowerOff() ||
            !this.emulationService.isUiInitialized() ||
            (!!currentSession && !supportsDBExport(currentSession.device))
        );
    }

    get powerDisabled(): boolean {
        const currentSession = this.emulationStateService.getCurrentSession();

        return currentSession ? quirkNoPoweroff(currentSession.device) : false;
    }

    async editSettings(): Promise<void> {
        const session = this.emulationStateService.getCurrentSession();
        if (!session) {
            return;
        }

        const oldSpeed = session.speed;
        const oldOrientation = session.deviceOrientation;

        const modal = await this.modalController.create({
            component: SessionSettingsComponent,
            backdropDismiss: false,
            componentProps: {
                session,
                availableDevices: [session.device],
                onSave: () => {
                    if (oldSpeed !== session.speed) this.performanceWatchdogService.reset();
                    if (oldOrientation !== session.deviceOrientation) {
                        this.canvasDisplayService.initialize(undefined, session);
                        this.canvasDisplayService.updateEmulationCanvas();
                    }

                    this.sessionService.updateSession(session);

                    modal.dismiss();
                },
                onCancel: () => modal.dismiss(),
            },
        });

        await this.popoverController.dismiss();
        await modal.present();
    }

    rotate(): void {
        const session = this.emulationStateService.getCurrentSession();
        if (!session) {
            return;
        }

        switch (session.deviceOrientation) {
            case DeviceOrientation.protrait:
            case undefined:
                session.deviceOrientation = DeviceOrientation.landscape90;
                break;

            case DeviceOrientation.landscape90:
                session.deviceOrientation = DeviceOrientation.portrait180;
                break;

            case DeviceOrientation.portrait180:
                session.deviceOrientation = DeviceOrientation.landscape270;
                break;

            case DeviceOrientation.landscape270:
                session.deviceOrientation = DeviceOrientation.protrait;
                break;
        }

        this.canvasDisplayService.initialize(undefined, session);
        this.canvasDisplayService.updateEmulationCanvas();

        this.sessionService.updateSession(session);
    }

    @Input()
    showHelp: () => void = () => undefined;
}
