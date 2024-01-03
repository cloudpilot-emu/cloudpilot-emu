import { AlertController, ModalController } from '@ionic/angular';
import { ChangeDetectorRef, Component, DoCheck } from '@angular/core';
import { DragDropClient, DragDropService } from '@pwa//service/drag-drop.service';
import { FileDescriptor, FileService } from '@pwa/service/file.service';
import { SessionSettings, SessionSettingsComponent } from '@pwa/component/session-settings/session-settings.component';

import { AlertService } from '@pwa/service/alert.service';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { EmulationService } from '@pwa/service/emulation.service';
import { EmulationStateService } from '@pwa/service/emulation-state.service';
import { HelpComponent } from '@pwa/component/help/help.component';
import { LinkApi } from '@pwa/service/link-api.service';
import { Router } from '@angular/router';
import { Session } from '@pwa/model/Session';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { SessionService } from '@pwa/service/session.service';
import { StorageService } from '@pwa/service/storage.service';
import { changeDetector } from '@pwa/helper/changeDetect';
import { debounce } from '@pwa/helper/debounce';
import deepEqual from 'deep-equal';
import { disambiguateName } from '@pwa/helper/disambiguate';

import helpUrl from '@assets/doc/sessions.md';

type mode = 'manage' | 'select-for-export';

@Component({
    selector: 'app-sessions',
    templateUrl: './sessions.page.html',
    styleUrls: ['./sessions.page.scss'],
})
export class SessionsPage implements DragDropClient, DoCheck {
    constructor(
        public sessionService: SessionService,
        private fileService: FileService,
        private alertController: AlertController,
        private alertService: AlertService,
        public emulationService: EmulationService,
        public emulationState: EmulationStateService,
        private storageService: StorageService,
        private modalController: ModalController,
        private linkApi: LinkApi,
        private router: Router,
        private cloudpilotService: CloudpilotService,
        private dragDropService: DragDropService,
        private cd: ChangeDetectorRef,
    ) {
        this.checkSessions = changeDetector(cd, [], () => this.sessionService.getSessions());
    }

    ngDoCheck(): void {
        this.checkSessions();
    }

    ionViewDidEnter(): void {
        this.linkApi.import.requestEvent.addHandler(this.handleLinkApiImportRequest);
        this.handleLinkApiImportRequest();

        this.dragDropService.registerClient(this);
    }

    ionViewWillLeave(): void {
        this.dragDropService.unregisterClient(this);
        this.linkApi.import.requestEvent.removeHandler(this.handleLinkApiImportRequest);
    }

    get sessions(): Array<Session> {
        return this.sessionService.getSessions().sort((a, b) => a.name.localeCompare(b.name));
    }

    get sessionCount(): number {
        return this.sessionService.getSessions().length;
    }

    @debounce()
    async deleteSession(session: Session): Promise<void> {
        const alert = await this.alertController.create({
            header: 'Warning',
            message: `Deleting the session '${session.name}' will remove all associated data. This cannot be undone. Are you sure you want to continue?`,
            buttons: [
                { text: 'Cancel', role: 'cancel' },
                { text: 'Delete', handler: () => this.sessionService.deleteSession(session) },
            ],
            backdropDismiss: false,
        });

        await alert.present();
    }

    @debounce()
    async editSession(session: Session): Promise<void> {
        const oldSession = { ...session };

        if ((await this.editSettings(session)) && !deepEqual(session, oldSession)) {
            await this.sessionService.updateSession(session);
        }
    }

    importSession(): void {
        this.fileService.openFile(this.processFile.bind(this));
    }

    @debounce()
    saveSession(session: Session): Promise<void> {
        return this.fileService.saveSession(session);
    }

    trackSessionBy(index: number, session: Session) {
        return session.id;
    }

    @debounce()
    async launchSession(session: Session) {
        this.lastSessionTouched = session.id;
        this.currentSessionOverride = session.id;
        this.cd.markForCheck();

        const launchSuccess = await this.emulationService.switchSession(session.id);
        this.currentSessionOverride = undefined;
        this.cd.markForCheck();

        if (!launchSuccess) return;

        void this.router.navigateByUrl('/tab/emulation');
    }

    @debounce()
    async resetSession(session: Session) {
        const running = session.id === this.emulationState.getCurrentSession()?.id;

        if (running) await this.emulationService.stop();

        await this.storageService.deleteStateForSession(session);

        await this.storageService.updateSessionPartial(session.id, { wasResetForcefully: true });

        if (running) await this.emulationService.switchSession(session.id);

        const alert = await this.alertController.create({
            header: 'Done',
            message: `Session ${session.name} has been reset.`,
            buttons: [{ text: 'Close', role: 'cancel' }],
            backdropDismiss: false,
        });

        await alert.present();
    }

    get currentSessionId(): number | undefined {
        return this.currentSessionOverride ?? this.emulationState.getCurrentSession()?.id;
    }

    async showHelp(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: helpUrl,
            },
        });
        await modal.present();
    }

    handleDragDropEvent(e: DragEvent): void | Promise<void> {
        this.fileService.openFromDrop(e, this.handleDropFiles.bind(this));
    }

    startMassExport(): void {
        this.selection.clear();
        this.mode = 'select-for-export';
    }

    @debounce()
    async onSelectionDone(): Promise<void> {
        const sessions = new Map(this.sessionService.getSessions().map((session) => [session.id, session]));
        const selectedSessions = Array.from(this.selection)
            .map((sessionId) => sessions.get(sessionId))
            .filter((session) => session !== undefined) as Array<Session>;

        this.mode = 'manage';

        if (selectedSessions.length === 1) {
            await this.fileService.saveSession(selectedSessions[0]);
        }

        if (selectedSessions.length > 1) {
            await this.fileService.saveSessions(selectedSessions);
        }
    }

    ionViewDidLeave(): void {
        this.mode = 'manage';
    }

    onSelectAll(): void {
        this.sessionService.getSessions().forEach((session) => this.selection.add(session.id));
    }

    toggleSelection(session: Session): void {
        if (this.selection.has(session.id)) this.selection.delete(session.id);
        else this.selection.add(session.id);
    }

    private handleDropFiles(files: Array<FileDescriptor>): void {
        if (files.length === 0) return;

        if (files.length > 1) {
            void this.alertService.errorMessage(
                'Drop a single ROM or session image in order to create a new session, or drop a zip archive with sessions to import.',
            );
            return;
        }

        void this.processFile(files[0]);
    }

    private async processFile(file: FileDescriptor): Promise<void> {
        if (!/\.(img|rom|bin|zip)$/i.test(file.name)) {
            void this.alertService.errorMessage(
                'Unsupported file suffix. Supported suffixes are .bin, .img, .rom. and .zip',
            );
            return;
        }

        let content: Uint8Array;
        try {
            content = await file.getContent();
        } catch (e) {
            console.warn(e);

            await this.alertService.errorMessage(`Unable to open ${file.name}.`);
            return;
        }

        if (file.name.endsWith('.zip')) {
            let install = false;
            await this.alertService.message(
                'Warning',
                'CloudpilotEmu will attempt to import all sessions in the selected zip archive. Are you sure you want to continue?',
                { Continue: () => (install = true) },
                'Cancel',
            );

            if (install) await this.sessionService.addSessionsFromArchive(content);
            return;
        }

        const sessionImage = (await this.cloudpilotService.cloudpilot).deserializeSessionImage<SessionMetadata>(
            content,
        );

        if (sessionImage) {
            const settings: SessionSettings = {
                name: this.disambiguateSessionName(sessionImage.metadata?.name ?? file.name),
                hotsyncName: sessionImage.metadata?.hotsyncName,
                device: sessionImage.deviceId,
                dontManageHotsyncName: sessionImage.metadata?.dontManageHotsyncName,
                speed: sessionImage.metadata?.speed,
                deviceOrientation: sessionImage.metadata?.deviceOrientation,
            };

            if (await this.editSettings(settings)) {
                const session = await this.sessionService.addSessionFromImage(sessionImage, settings.name, settings);

                this.lastSessionTouched = session.id;
            }
        } else {
            const romInfo = (await this.cloudpilotService.cloudpilot).getRomInfo(content);

            if (!romInfo) {
                void this.alertService.errorMessage(`Not a valid session file or ROM image.`);
                return;
            }

            if (romInfo.supportedDevices.length === 0) {
                void this.alertService.errorMessage(`No supported device for this ROM.`);
                return;
            }

            const settings: SessionSettings = {
                name: this.disambiguateSessionName(file.name),
                hotsyncName: '',
                device: romInfo.supportedDevices[0],
            };

            if (await this.editSettings(settings, romInfo.supportedDevices)) {
                const session = await this.sessionService.addSessionFromRom(
                    content,
                    settings.name,
                    settings.device,
                    settings,
                );

                this.lastSessionTouched = session.id;
            }
        }
    }

    private editSettings(session: SessionSettings, availableDevices = [session.device]): Promise<boolean> {
        return new Promise((resolve) => {
            let modal: HTMLIonModalElement;

            void this.modalController
                .create({
                    component: SessionSettingsComponent,
                    backdropDismiss: false,
                    componentProps: {
                        session,
                        availableDevices,
                        onSave: () => {
                            void modal.dismiss();
                            resolve(true);
                        },
                        onCancel: () => {
                            void modal.dismiss();
                            resolve(false);
                        },
                    },
                })
                .then((m) => {
                    modal = m;
                    void modal.present();
                });
        });
    }

    private disambiguateSessionName(originalName: string) {
        const sessions = this.sessionService.getSessions();

        return disambiguateName(originalName, (x) => sessions.some((session) => session.name === x));
    }

    private handleLinkApiImportRequest = (): void => {
        if (!this.linkApi.import.hasPendingRequest()) {
            return;
        }

        const url = this.linkApi.import.receivePendingUrl();
        if (!url) {
            return;
        }

        void this.alertService.message(
            'Import request',
            `Do you want to import<br>${url} ?`,
            {
                OK: () => this.fileService.openUrl(url, this.processFile.bind(this)),
            },
            'Cancel',
        );
    };

    lastSessionTouched = -1;

    mode: mode = 'manage';

    selection = new Set<number>();

    private checkSessions: () => void;
    private currentSessionOverride: number | undefined;
}
