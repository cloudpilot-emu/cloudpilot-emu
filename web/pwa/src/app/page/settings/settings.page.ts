import { Component, OnInit } from '@angular/core';
import { UntypedFormControl, UntypedFormGroup } from '@angular/forms';
import helpUrl from '@assets/doc/settings.md';
import { isIOS, isIOSNative } from '@common/helper/browser';
import { ModalController } from '@ionic/angular';
import { MutexInterface } from 'async-mutex';
import { environment } from 'pwa/src/environments/environment';

import { HelpComponent } from '@pwa/component/help/help.component';
import {
    IndicatorFixMode,
    applyHomeIndicatorFix,
    getIndicatorFixMode,
    isIndicatorFixApplicable,
} from '@pwa/helper/homeIndicatorFix';
import { validateProxyAddress } from '@pwa/helper/proxyAddress';
import { NetworkRedirectionMode } from '@pwa/model/Kvs';
import { AlertService } from '@pwa/service/alert.service';
import { ClipboardService } from '@pwa/service/clipboard.service';
import { FeatureService } from '@pwa/service/feature.service';
import { KvsService } from '@pwa/service/kvs.service';
import { NetworkBackendFactory } from '@pwa/service/network-backend/network-backend-factory.service';

const enum fields {
    volume = 'volume',
    showStatistics = 'showStatistics',
    clipboardIntegration = 'clipboardIntegration',
    networkRedirection = 'networkRedirection',
    networkRedirectionMode = 'networkRedirectionMode',
    proxyServer = 'proxyServer',
    runHidden = 'runHidden',
    autoLockUI = 'autoLockUI',
    enableRemoteInstall = 'enableRemoteInstall',
    audioOnStart = 'audioOnStart',
    snapshotIntegrityCheck = 'snapshotIntegrityCheck',
    indicatorFixMode = 'indicatorFixMode',
}
@Component({
    selector: 'app-settings',
    templateUrl: './settings.page.html',
    styleUrls: ['./settings.page.scss'],
    standalone: false,
})
export class SettingsPage implements OnInit {
    constructor(
        private modalController: ModalController,
        private kvsService: KvsService,
        public clipboardService: ClipboardService,
        private alertService: AlertService,
        private networkBackendFactory: NetworkBackendFactory,
        private featureService: FeatureService,
    ) {}

    ngOnInit(): void {
        this.createFormGroup();
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

    ionViewWillEnter(): void {
        this.mutexReleasePromise = this.kvsService.mutex.acquire();
    }

    async ionViewWillLeave(): Promise<void> {
        if (
            this.formGroup.get(fields.networkRedirection)?.value &&
            this.formGroup.get(fields.networkRedirectionMode)?.value === 'proxy' &&
            !this.formGroup.get(fields.proxyServer)?.valid
        ) {
            void this.alertService.message(
                'Invalid proxy server',
                'The proxy server you specified is invalid. Network redirection will be disabled.',
            );

            this.formGroup.get(fields.networkRedirection)!.setValue(false);
        }

        if (!this.formGroup.get('networkRedirection')?.value) {
            this.formGroup.get('proxyServer')?.setValue(this.kvsService.kvs.proxyServer);
        }

        await this.kvsService.set({
            volume: this.formGroup.get(fields.volume)!.value,
            showStatistics: this.formGroup.get(fields.showStatistics)!.value,
            clipboardIntegration: this.formGroup.get(fields.clipboardIntegration)?.value,
            networkRedirection: this.formGroup.get(fields.networkRedirection)?.value,
            networkRedirectionMode: this.featureService.nativeNetworkIntegration
                ? (this.formGroup.get(fields.networkRedirectionMode)?.value ?? 'proxy')
                : 'proxy',
            proxyServer: this.formGroup.get(fields.proxyServer)?.value,
            runHidden: this.formGroup.get(fields.runHidden)?.value,
            autoLockUI: this.formGroup.get(fields.autoLockUI)?.value,
            enableRemoteInstall: this.formGroup.get(fields.enableRemoteInstall)?.value,
            enableAudioOnFirstInteraction: this.formGroup.get(fields.audioOnStart)?.value,
            snapshotIntegrityCheck: this.formGroup.get(fields.snapshotIntegrityCheck)?.value,
        });

        if (this.mutexReleasePromise) {
            (await this.mutexReleasePromise)();
        }
    }

    async testProxyConnection(): Promise<void> {
        if (this.connectionTestInProgress) return;

        this.connectionTestInProgress = true;

        try {
            const handshakeResult = await this.networkBackendFactory
                .createBackendProxy(this.formGroup.get(fields.proxyServer)?.value)
                .handshake(0);

            switch (handshakeResult.status) {
                case 'failed':
                    void this.alertService.errorMessage('Unable to connect to proxy');
                    break;

                case 'version_mismatch':
                    void this.alertService.proxyVersionMismatchError();
                    break;

                default:
                    void this.alertService.message('Success', 'Connection to proxy successful.');
            }
        } finally {
            this.connectionTestInProgress = false;
        }
    }

    get showProxyServer(): boolean {
        return (
            this.formGroup.get(fields.networkRedirection)?.value &&
            this.formGroup.get(fields.networkRedirectionMode)?.value !== 'native'
        );
    }

    get showRedirectionMode(): boolean {
        return this.formGroup.get(fields.networkRedirection)?.value && this.featureService.nativeNetworkIntegration;
    }

    get proxyServerValid(): boolean {
        const fieldProxyServer = this.formGroup.get(fields.proxyServer);

        return !!(fieldProxyServer?.value && fieldProxyServer?.valid);
    }

    get showDebugOptions(): boolean {
        return environment.debug;
    }

    get isIndicatorFixApplicable(): boolean {
        return isIndicatorFixApplicable();
    }

    get featureRunHidden(): boolean {
        return this.featureService.runHidden;
    }

    private createFormGroup() {
        this.formGroup = new UntypedFormGroup({
            [fields.volume]: new UntypedFormControl(this.kvsService.kvs.volume),
            [fields.showStatistics]: new UntypedFormControl(this.kvsService.kvs.showStatistics),
            [fields.runHidden]: new UntypedFormControl(this.kvsService.kvs.runHidden),
            [fields.clipboardIntegration]: new UntypedFormControl(this.kvsService.kvs.clipboardIntegration),
            [fields.networkRedirection]: new UntypedFormControl(this.kvsService.kvs.networkRedirection),
            [fields.networkRedirectionMode]: new UntypedFormControl(this.kvsService.kvs.networkRedirectionMode),
            [fields.proxyServer]: new UntypedFormControl(this.kvsService.kvs.proxyServer, {
                validators: [validateProxyAddress],
            }),
            [fields.autoLockUI]: new UntypedFormControl(this.kvsService.kvs.autoLockUI),
            [fields.enableRemoteInstall]: new UntypedFormControl(this.kvsService.kvs.enableRemoteInstall),
            [fields.audioOnStart]: new UntypedFormControl(this.kvsService.kvs.enableAudioOnFirstInteraction),
            [fields.snapshotIntegrityCheck]: new UntypedFormControl(this.kvsService.kvs.snapshotIntegrityCheck),
            [fields.indicatorFixMode]: new UntypedFormControl(getIndicatorFixMode()),
        });

        this.formGroup.get(fields.audioOnStart)?.valueChanges.subscribe(this.onAudioOnStartChange);
        this.formGroup.get(fields.indicatorFixMode)?.valueChanges.subscribe(this.onIndicatorFixModeChange);
    }

    private onAudioOnStartChange = (audioOnStart: boolean) => {
        this.kvsService.kvs.enableAudioOnFirstInteraction = audioOnStart;

        if (isIOS && isIOSNative) {
            void this.alertService.message(
                'Enable audio on start',
                `
                    This option will take effect the next time the app restarts.
                `,
            );

            return;
        }

        if (!audioOnStart) return;

        void this.alertService.message(
            'Enable audio on start',
            `
                Audio will automatically turn on after the first interaction with the application
                (i.e. touch, click or keyboard event).
                <br><br>
                This option will take effect the next time CloudpilotEmu reloads.
            `,
        );
    };

    readonly networkRedirectionModes: Array<[NetworkRedirectionMode, string]> = [
        ['native', 'Native'],
        ['proxy', 'Proxy'],
    ];

    private onIndicatorFixModeChange = (mode: IndicatorFixMode) => applyHomeIndicatorFix(mode);

    readonly indicatorFixNone = IndicatorFixMode.none;
    readonly indicatorFixPortrait = IndicatorFixMode.portrait;
    readonly indicatorFixLandscape = IndicatorFixMode.landscape;
    readonly indicatorFixAll = IndicatorFixMode.all;

    formGroup!: UntypedFormGroup;
    private connectionTestInProgress = false;
    private mutexReleasePromise: Promise<MutexInterface.Releaser> | undefined;
}
