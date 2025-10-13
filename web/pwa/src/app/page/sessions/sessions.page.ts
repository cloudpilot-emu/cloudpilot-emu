import {
    ChangeDetectionStrategy,
    ChangeDetectorRef,
    Component,
    Input,
    OnInit,
    Signal,
    computed,
    signal,
} from '@angular/core';
import { Router } from '@angular/router';
import helpUrl from '@assets/doc/sessions.md';
import { isIOS, isIOSNative } from '@common/helper/browser';
import { nandSize } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { LoadingController, ModalController, PopoverController } from '@ionic/angular';

import { DragDropClient, DragDropService } from '@pwa//service/drag-drop.service';
import { HelpComponent } from '@pwa/component/help/help.component';
import { SessionSettings, SessionSettingsComponent } from '@pwa/component/session-settings/session-settings.component';
import { debounce } from '@pwa/helper/debounce';
import { disambiguateName } from '@pwa/helper/disambiguate';
import { memoize } from '@pwa/helper/memoize';
import { Session, buildSettings, mergeSettings, settingsFromMetadata, settingsFromSession } from '@pwa/model/Session';
import { AlertService } from '@pwa/service/alert.service';
import { CloudpilotService } from '@pwa/service/cloudpilot.service';
import { EmulationContextService } from '@pwa/service/emulation-context.service';
import { EmulationService } from '@pwa/service/emulation.service';
import { FileDescriptor, FileService } from '@pwa/service/file.service';
import { LinkApi } from '@pwa/service/link-api.service';
import { SessionService } from '@pwa/service/session.service';
import { StorageService } from '@pwa/service/storage.service';

import { ActionMenuComponent } from './action-menu/action-menu.component';

type Mode = 'manage' | 'select-for-export' | 'select-for-delete';

@Component({
    selector: 'app-sessions',
    templateUrl: './sessions.page.html',
    styleUrls: ['./sessions.page.scss'],
    changeDetection: ChangeDetectionStrategy.OnPush,
    standalone: false,
})
export class SessionsPage implements DragDropClient, OnInit {
    constructor(
        public sessionService: SessionService,
        private fileService: FileService,
        private alertService: AlertService,
        public emulationService: EmulationService,
        public emulationContext: EmulationContextService,
        private storageService: StorageService,
        private modalController: ModalController,
        private linkApi: LinkApi,
        private router: Router,
        private cloudpilotService: CloudpilotService,
        private dragDropService: DragDropService,
        private popoverController: PopoverController,
        private loadingController: LoadingController,
        private cd: ChangeDetectorRef,
    ) {
        this.currentSessionId = computed(() => this.currentSessionOverride() ?? this.emulationContext.session()?.id);
    }

    ngOnInit(): void {
        if (this.selfReference) this.selfReference.ref = this;
    }

    ionViewDidEnter_(): void {
        this.linkApi.import.requestEvent.addHandler(this.handleLinkApiImportRequest);
        this.handleLinkApiImportRequest();

        this.dragDropService.registerClient(this);
    }

    ionViewWillLeave(): void {
        this.dragDropService.unregisterClient(this);
        this.linkApi.import.requestEvent.removeHandler(this.handleLinkApiImportRequest);
    }

    ionViewDidLeave(): void {
        this.mode = 'manage';
        this.cd.detectChanges();
    }

    private memoizedSessions = memoize((sessions: Array<Session>) =>
        [...sessions].sort((a, b) => a.name.localeCompare(b.name)),
    );

    get sessions(): Array<Session> {
        return this.memoizedSessions(this.sessionService.sessions());
    }

    get sessionCount(): number {
        return this.sessionService.sessions().length;
    }

    @debounce()
    async deleteSession(session: Session): Promise<void> {
        let abort = true;

        await this.alertService.message(
            'Warning',
            `Deleting the session '${session.name}' will remove all associated data. This cannot be undone. Are you sure you want to continue?`,
            {
                Delete: () => (abort = false),
            },
            'Cancel',
        );

        if (abort) return;

        const loader = await this.loadingController.create({ message: 'Deleting...' });
        await loader.present();

        try {
            await this.sessionService.deleteSession(session);
        } finally {
            void loader.dismiss();
        }
    }

    @debounce()
    async editSession(session: Session): Promise<void> {
        const settings = settingsFromSession(session);

        if ((await this.editSettings(settings, session.device)) !== undefined) {
            await this.sessionService.updateSession(mergeSettings(session, settings));
        }
    }

    importSession(): void {
        this.fileService.openFile(this.processFile.bind(this));
    }

    @debounce()
    saveSession(session: Session): Promise<void> {
        return this.sessionService.saveSession(session);
    }

    trackSessionBy(index: number, session: Session) {
        return session.id;
    }

    @debounce()
    async launchSession(session: Session) {
        this.lastSessionTouched = session.id;
        this.currentSessionOverride.set(session.id);

        const launchSuccess = await this.emulationService.switchSession(session.id);
        this.currentSessionOverride.set(undefined);

        if (!launchSuccess) return;

        void this.router.navigateByUrl('/tab/emulation');
    }

    @debounce()
    async resetSession(session: Session) {
        const running = session.id === this.emulationContext.session()?.id;

        if (running) await this.emulationService.stop();

        await this.storageService.deleteStateForSession(session);
        await this.storageService.updateSessionPartial(session.id, { wasResetForcefully: true });

        if (running) await this.emulationService.switchSession(session.id);

        await this.alertService.message('Done', `Session ${session.name} has been reset.`);
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

    startMassDelete(): void {
        this.selection.clear();
        this.mode = 'select-for-delete';
    }

    @debounce()
    async onSelectionDone(): Promise<void> {
        const sessions = new Map(this.sessionService.sessions().map((session) => [session.id, session]));
        const selectedSessions = Array.from(this.selection)
            .map((sessionId) => sessions.get(sessionId))
            .filter((session) => session !== undefined) as Array<Session>;

        const wasMode = this.mode;
        this.mode = 'manage';

        switch (wasMode) {
            case 'select-for-delete':
                await this.executeMassDelete(selectedSessions);
                break;

            case 'select-for-export':
                await this.executeMassExport(selectedSessions);
                break;
        }
    }

    onSelectAll(): void {
        this.sessionService.sessions().forEach((session) => this.selection.add(session.id));
    }

    toggleSelection(session: Session): void {
        if (this.selection.has(session.id)) this.selection.delete(session.id);
        else this.selection.add(session.id);
    }

    @debounce()
    async openActionMenu(e: MouseEvent, reference: 'event' | 'trigger' = 'trigger'): Promise<void> {
        const popover = await this.popoverController.create({
            component: ActionMenuComponent,
            event: e,
            componentProps: {
                sessionCount: this.sessionCount,
                onExport: () => {
                    void popover.dismiss();
                    this.startMassExport();
                    this.cd.markForCheck();
                },
                onDelete: () => {
                    void popover.dismiss();
                    this.startMassDelete();
                    this.cd.markForCheck();
                },
                onHelp: () => {
                    void popover.dismiss();
                    void this.showHelp();
                },
            },
            arrow: false,
            translucent: true,
            reference,
        });

        void popover.present();
    }

    onContextMenu(e: MouseEvent): void {
        if (this.mode !== 'manage') return;

        e.preventDefault();

        void this.openActionMenu(e, 'event');
    }

    get showPalmdbLink(): boolean {
        return !(isIOS && isIOSNative);
    }

    private async executeMassExport(selectedSessions: Array<Session>): Promise<void> {
        if (selectedSessions.length === 1) {
            await this.sessionService.saveSession(selectedSessions[0]);
        }

        if (selectedSessions.length > 1) {
            await this.sessionService.saveSessions(selectedSessions);
        }
    }

    private async executeMassDelete(selectedSessions: Array<Session>): Promise<void> {
        if (selectedSessions.length === 1) {
            await this.deleteSession(selectedSessions[0]);
        }

        if (selectedSessions.length > 1) {
            let abort = true;

            await this.alertService.message(
                'Warning',
                `
                    You are about to delete ${selectedSessions.length} sessions, which will destroy all data associated with these sessions.
                    This cannot be undone. Are you sure you want to continue?`,
                {
                    Delete: () => (abort = false),
                },
                'Cancel',
            );

            if (abort) return;

            const loader = await this.loadingController.create({ message: 'Deleting...' });
            await loader.present();

            try {
                for (const session of selectedSessions) await this.sessionService.deleteSession(session);
            } finally {
                void loader.dismiss();
            }
        }
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
        const loader = await this.loadingController.create();
        await loader.present();

        try {
            content = await file.getContent();
        } catch (e) {
            console.warn(e);

            await this.alertService.errorMessage(`Unable to open ${file.name}.`);
            return;
        } finally {
            void loader.dismiss();
        }

        if (file.name.endsWith('.zip')) {
            let install = false;
            await this.alertService.message(
                'Mass import',
                `
                    CloudpilotEmu will attempt to import all sessions in the selected zip archive.
                    Importing large archives (several 100MB) requires a lot of memory and may crash the browser if the archive is too large.
                    Are you sure you want to continue?
                `,
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
            const settings = {
                ...settingsFromMetadata(sessionImage.engine, sessionImage.metadata),
                name: this.disambiguateSessionName(sessionImage.metadata?.name ?? file.name),
            };

            if ((await this.editSettings(settings, sessionImage.deviceId)) !== undefined) {
                const session = await this.sessionService.addSessionFromImage(sessionImage, settings);

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

            const settings = {
                ...buildSettings({ engine: romInfo.engine, name: this.disambiguateSessionName(file.name) }),
                name: this.disambiguateSessionName(file.name),
            };

            const [device, nand] =
                (await this.editSettings(
                    settings,
                    romInfo.supportedDevices[0],
                    romInfo.supportedDevices,
                    nandSize(romInfo.supportedDevices[0]),
                )) ?? [];

            if (device !== undefined) {
                const session = await this.sessionService.addSessionFromRom(content, device, settings, nand);

                this.lastSessionTouched = session.id;
            }
        }
    }

    private editSettings(
        settings: SessionSettings,
        device: DeviceId,
        availableDevices = [device],
        selectNandSize?: number,
    ): Promise<[DeviceId, Uint8Array | undefined] | undefined> {
        return new Promise((resolve) => {
            let modal: HTMLIonModalElement;

            void this.modalController
                .create({
                    component: SessionSettingsComponent,
                    backdropDismiss: false,
                    componentProps: {
                        settings,
                        availableDevices,
                        device,
                        selectNandSize,
                        onSave: (device: DeviceId, nand: Uint8Array | undefined) => {
                            void modal.dismiss();
                            resolve([device, nand]);
                        },
                        onCancel: () => {
                            void modal.dismiss();
                            resolve(undefined);
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
        const sessions = this.sessionService.sessions();

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

    @Input()
    selfReference: { ref: SessionsPage } | undefined;

    lastSessionTouched = -1;

    mode: Mode = 'manage';

    selection = new Set<number>();

    currentSessionId: Signal<number | undefined>;

    private currentSessionOverride = signal<number | undefined>(undefined);
}
