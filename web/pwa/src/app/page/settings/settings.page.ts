import { Component, OnInit } from '@angular/core';
import { UntypedFormControl, UntypedFormGroup } from '@angular/forms';

import { AlertService } from '@pwa/service/alert.service';
import { ClipboardService } from '@pwa/service/clipboard.service';
import { HelpComponent } from '@pwa/component/help/help.component';
import { KvsService } from '@pwa/service/kvs.service';
import { ModalController } from '@ionic/angular';
import { MutexInterface } from 'async-mutex';
import { ProxyService } from '@pwa/service/proxy.service';
import { environment } from 'pwa/src/environments/environment';
import { validateProxyAddress } from '@pwa/helper/proxyAddress';
import { getReducedAnimations, setReducedAnimations } from '@pwa/helper/reducedAnimations';

import helpUrl from '@assets/doc/settings.md';
import {
    IndicatorFixMode,
    applyHomeIndicatorFix,
    getIndicatorFixMode,
    isIndicatorFixApplicable,
} from '@pwa/helper/homeIndicatorFix';
import { FeatureService } from '@pwa/service/feature.service';
import { isIOS, isIOSNative } from '@common/helper/browser';

const enum fields {
    volume = 'volume',
    showStatistics = 'showStatistics',
    clipboardIntegration = 'clipboardIntegration',
    networkRedirection = 'networkRedirection',
    proxyServer = 'proxyServer',
    runHidden = 'runHidden',
    autoLockUI = 'autoLockUI',
    enableRemoteInstall = 'enableRemoteInstall',
    audioOnStart = 'audioOnStart',
    snapshotIntegrityCheck = 'snapshotIntegrityCheck',
    reducedAnimations = 'reducedAnimations',
    indicatorFixMode = 'indicatorFixMode',
}
@Component({
    selector: 'app-settings',
    templateUrl: './settings.page.html',
    styleUrls: ['./settings.page.scss'],
})
export class SettingsPage implements OnInit {
    constructor(
        private modalController: ModalController,
        private kvsService: KvsService,
        public clipboardService: ClipboardService,
        private alertService: AlertService,
        private proxyService: ProxyService,
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
        if (this.formGroup.get(fields.networkRedirection)?.value && !this.formGroup.get(fields.proxyServer)?.valid) {
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
            proxyServer: this.formGroup.get(fields.proxyServer)?.value,
            runHidden: this.formGroup.get(fields.runHidden)?.value,
            autoLockUI: this.formGroup.get(fields.autoLockUI)?.value,
            enableRemoteInstall: this.formGroup.get(fields.enableRemoteInstall)?.value,
            enableAudioOnFirstInteraction: this.formGroup.get(fields.audioOnStart)?.value,
            snapshotIntegrityCheck: this.formGroup.get(fields.snapshotIntegrityCheck)?.value,
        });

        setReducedAnimations(this.formGroup.get(fields.reducedAnimations)?.value);

        if (this.mutexReleasePromise) {
            (await this.mutexReleasePromise)();
        }
    }

    async testProxyConnection(): Promise<void> {
        if (this.connectionTestInProgress) return;

        this.connectionTestInProgress = true;

        try {
            const handshakeResult = await this.proxyService.handshake(this.formGroup.get(fields.proxyServer)?.value, 0);

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
        return this.formGroup.get(fields.networkRedirection)?.value;
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
            [fields.proxyServer]: new UntypedFormControl(this.kvsService.kvs.proxyServer, {
                validators: [validateProxyAddress],
            }),
            [fields.autoLockUI]: new UntypedFormControl(this.kvsService.kvs.autoLockUI),
            [fields.enableRemoteInstall]: new UntypedFormControl(this.kvsService.kvs.enableRemoteInstall),
            [fields.audioOnStart]: new UntypedFormControl(this.kvsService.kvs.enableAudioOnFirstInteraction),
            [fields.snapshotIntegrityCheck]: new UntypedFormControl(this.kvsService.kvs.snapshotIntegrityCheck),
            [fields.reducedAnimations]: new UntypedFormControl(getReducedAnimations()),
            [fields.indicatorFixMode]: new UntypedFormControl(getIndicatorFixMode()),
        });

        this.formGroup.get(fields.audioOnStart)?.valueChanges.subscribe(this.onAudioOnStartChange);
        this.formGroup.get(fields.reducedAnimations)?.valueChanges.subscribe(this.onReducedAnimationsChange);
        this.formGroup.get(fields.indicatorFixMode)?.valueChanges.subscribe(this.onIndicatorFixModeChange);
    }

    private onAudioOnStartChange = (audioOnStart: boolean) => {
        this.kvsService.kvs.enableAudioOnFirstInteraction = audioOnStart;

        if (isIOS && isIOSNative) {
            void this.alertService.message(
                'Enable audio on start',
                `
                    This option will take effect the next time the app restarts.
                    <br><br>
                    Please note that
                    there is an iOS bug that may cause the app to blink shortly when it is resumed
                    from the background if audio is enabled (even if it was muted again).
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

    private onReducedAnimationsChange = (reducedAnimations: boolean) => {
        setReducedAnimations(reducedAnimations);

        if (reducedAnimations) {
            void this.alertService.message(
                'Reduce animations',
                `
                Animations will be slightly reduced in order to provide a snappier UI. This
                reduces UI lag on iOS devices with large screens.
                <br><br>
                This option will take effect the next time
                CloudpilotEmu reloads.
            `,
            );
        } else {
            void this.alertService.message(
                'Reduce animations',
                `
                This option will only take effect the next time CloudpilotEmu reloads.
            `,
            );
        }
    };

    private onIndicatorFixModeChange = (mode: IndicatorFixMode) => applyHomeIndicatorFix(mode);

    readonly indicatorFixNone = IndicatorFixMode.none;
    readonly indicatorFixPortrait = IndicatorFixMode.portrait;
    readonly indicatorFixLandscape = IndicatorFixMode.landscape;
    readonly indicatorFixAll = IndicatorFixMode.all;

    formGroup!: UntypedFormGroup;
    private connectionTestInProgress = false;
    private mutexReleasePromise: Promise<MutexInterface.Releaser> | undefined;
}
