import { Component, Input } from '@angular/core';
import { PalmButton } from '@common/bridge/Cloudpilot';
import { quirkNoHotsync, quirkNoPoweroff, slotType } from '@common/helper/deviceProperties';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { SlotType } from '@common/model/SlotType';
import {
    ActionSheetController,
    AlertController,
    LoadingController,
    ModalController,
    PopoverController,
} from '@ionic/angular';

import { SessionSettingsComponent } from '@pwa/component/session-settings/session-settings.component';
import { filenameForScreenshot } from '@pwa/helper/filename';
import { SessionSettings, mergeSettings, settingsFromSession } from '@pwa/model/Session';
import { StorageCard } from '@pwa/model/StorageCard';
import { AlertService } from '@pwa/service/alert.service';
import { AudioService } from '@pwa/service/audio.service';
import { BackupService } from '@pwa/service/backup.service';
import { ButtonService } from '@pwa/service/button.service';
import { CanvasDisplayService } from '@pwa/service/canvas-display.service';
import { EmulationContextService } from '@pwa/service/emulation-context.service';
import { EmulationService } from '@pwa/service/emulation.service';
import { ErrorService } from '@pwa/service/error.service';
import { FileService } from '@pwa/service/file.service';
import { KvsService } from '@pwa/service/kvs.service';
import { NativeSupportService } from '@pwa/service/native-support.service';
import { PerformanceWatchdogService } from '@pwa/service/performance-watchdog.service';
import { SessionService } from '@pwa/service/session.service';
import { StorageCardService } from '@pwa/service/storage-card.service';
import { StorageService } from '@pwa/service/storage.service';

function rotate(oldOrientation: DeviceOrientation): DeviceOrientation {
    switch (oldOrientation) {
        case DeviceOrientation.portrait:
        case undefined:
            return DeviceOrientation.landscape90;

        case DeviceOrientation.landscape90:
            return DeviceOrientation.portrait180;

        case DeviceOrientation.portrait180:
            return DeviceOrientation.landscape270;

        case DeviceOrientation.landscape270:
            return DeviceOrientation.portrait;

        default:
            throw new Error('cannot happen');
    }
}

function performanceSettingsChanged(settingsOld: SessionSettings, settingsNew: SessionSettings): boolean {
    if (settingsOld.engine === 'cloudpilot' && settingsNew.engine === 'cloudpilot') {
        return settingsOld.speed !== settingsNew.speed;
    }

    if (settingsOld.engine === 'uarm' && settingsNew.engine === 'uarm') {
        return (
            settingsOld.targetMips !== settingsNew.targetMips ||
            settingsOld.maxHostLoad !== settingsNew.maxHostLoad ||
            settingsOld.warnSlowdownThreshold !== settingsNew.warnSlowdownThreshold
        );
    }

    throw new Error('unreachable');
}

@Component({
    selector: 'app-emulation-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
    standalone: false,
})
export class ContextMenuComponent {
    constructor(
        public emulationService: EmulationService,
        private emulationContext: EmulationContextService,
        private backupService: BackupService,
        private buttonService: ButtonService,
        private actionSheetController: ActionSheetController,
        private popoverController: PopoverController,
        private audioService: AudioService,
        private kvsService: KvsService,
        private modalController: ModalController,
        private sessionService: SessionService,
        private performanceWatchdogService: PerformanceWatchdogService,
        private canvasDisplayService: CanvasDisplayService,
        private nativeSupportService: NativeSupportService,
        private storageCardService: StorageCardService,
        private alertService: AlertService,
        private alertController: AlertController,
        private storageService: StorageService,
        private errorService: ErrorService,
        private loadingController: LoadingController,
        private fileService: FileService,
    ) {}

    async reset(): Promise<void> {
        void this.popoverController.dismiss();

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

        void sheet.present();
    }

    power(): void {
        this.buttonService.engage(PalmButton.power);

        void this.popoverController.dismiss();
    }

    hotsync(): void {
        this.buttonService.engage(PalmButton.cradle);

        void this.popoverController.dismiss();
    }

    help(): void {
        this.showHelp();

        void this.popoverController.dismiss();
    }

    async backup(): Promise<void> {
        void this.popoverController.dismiss();

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

        void sheet.present();
    }

    get isMuted(): boolean {
        return !this.audioService.isInitialized() || this.audioService.isMuted();
    }

    get isInitialized(): boolean {
        return this.audioService.isInitialized();
    }

    get isAudioOff(): boolean {
        return this.kvsService.kvs.volume <= 0;
    }

    get audioRequiresReactivate(): boolean {
        return this.audioService.reactivateRequired();
    }

    mute(muted: boolean): void {
        if (muted) {
            this.audioService.mute(true);
        } else {
            if (this.audioService.reactivateRequired()) {
                this.audioService.reactivate();
            } else if (this.audioService.isInitialized()) {
                this.audioService.mute(false);
            } else {
                void this.audioService.initialize();
            }
        }
    }

    get backupDisabled(): boolean {
        return (
            !this.emulationService.isRunning() ||
            this.emulationService.isPowerOff() ||
            !this.emulationService.isUiInitialized()
        );
    }

    get powerDisabled(): boolean {
        const currentSession = this.emulationContext.session();

        return currentSession ? quirkNoPoweroff(currentSession.device) : false;
    }

    get hotsyncDisabled(): boolean {
        const currentSession = this.emulationContext.session();

        return currentSession ? quirkNoHotsync(currentSession.device) : false;
    }

    get hasActiveSession(): boolean {
        return !!this.emulationContext.session();
    }

    get supportsStorageCard(): boolean {
        const currentSession = this.emulationContext.session();
        if (!currentSession) return false;

        return slotType(currentSession.device) !== SlotType.none;
    }

    get hasMemorystick(): boolean {
        const currentSession = this.emulationContext.session();
        if (!currentSession) return false;

        return slotType(currentSession.device) === SlotType.memorystick;
    }

    get hasMountedCard(): boolean {
        return this.emulationContext.session()?.mountedCard !== undefined;
    }

    async editSettings(): Promise<void> {
        const session = this.emulationContext.session();
        if (!session) {
            return;
        }

        const settings = settingsFromSession(session);
        const oldSettings = { ...settings };

        const modal = await this.modalController.create({
            component: SessionSettingsComponent,
            backdropDismiss: false,
            componentProps: {
                settings,
                availableDevices: [session.device],
                device: session.device,
                onSave: async () => {
                    const updatedSession = mergeSettings(session, settings);

                    if (performanceSettingsChanged(oldSettings, settings)) this.performanceWatchdogService.reset();
                    if (oldSettings.deviceOrientation !== settings.deviceOrientation) {
                        void this.canvasDisplayService.initialize(undefined, updatedSession);
                        this.canvasDisplayService.updateEmulationCanvas();
                    }

                    await this.sessionService.updateSession(updatedSession);

                    void modal.dismiss();
                },
                onCancel: () => modal.dismiss(),
            },
        });

        await this.popoverController.dismiss();
        await modal.present();
    }

    async rotate(): Promise<void> {
        const oldSession = this.emulationContext.session();
        if (!oldSession) return;

        await this.storageService.updateSessionPartial(oldSession.id, {
            deviceOrientation: rotate(oldSession.deviceOrientation),
        });

        const session = this.emulationContext.session();

        await this.canvasDisplayService.initialize(undefined, session);
        this.canvasDisplayService.updateEmulationCanvas();
    }

    async insertCard(): Promise<void> {
        void this.popoverController.dismiss();

        const deviceId = this.emulationContext.session()?.device;
        if (deviceId === undefined) return;

        const eligibleCards = (
            await Promise.all(
                this.storageCardService
                    .cards()
                    .map(
                        (card): Promise<[StorageCard, boolean]> =>
                            this.nativeSupportService
                                .deviceSupportsCard(deviceId, card.size)
                                .then((supported) => [card, supported]),
                    ),
            )
        )
            .filter(([card, supported]) => supported && !this.storageCardService.mountedInSession(card.id))
            .map(([card]) => card);

        if (eligibleCards.length === 0) {
            void this.alertService.message(
                'No image available',
                `There is no ${
                    this.hasMemorystick ? 'memory stick' : 'SD card'
                } image available. Please go to the "Storage" tab in order to create new one.`,
            );

            return;
        }

        const alert = await this.alertController.create({
            header: 'Choose image for insertion',
            buttons: [
                { text: 'Cancel', role: 'cancel' },
                { text: 'Insert', handler: (id) => this.storageCardService.insertCard(id) },
            ],
            inputs: eligibleCards.map((card, index) => ({
                label: card.name,
                type: 'radio',
                value: card.id,
                checked: index === 0,
            })),
            backdropDismiss: false,
        });

        await alert.present();
    }

    async ejectCard(): Promise<void> {
        try {
            await this.storageCardService.ejectCurrentCard();
        } catch (e) {
            this.errorService.fatalBug(e instanceof Error ? e.message : 'eject failed');
        }
        void this.popoverController.dismiss();
    }

    async saveScreenshot(): Promise<void> {
        const loader = await this.loadingController.create();
        try {
            await loader.present();

            const session = this.emulationContext.session();
            if (!session) return;

            const screenshot = await this.canvasDisplayService.screenshot();
            if (!screenshot) throw new Error(`screenshot is undefined`);

            this.fileService.saveBlob(filenameForScreenshot(session), screenshot);
        } catch (e) {
            console.error(e);

            void this.alertService.errorMessage('Failed to create screenshot.');
        } finally {
            await loader.dismiss();
        }
    }

    @Input()
    showHelp: () => void = () => undefined;
}
