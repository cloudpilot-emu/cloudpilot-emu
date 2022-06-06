import { Component, OnInit } from '@angular/core';
import { FormControl, FormGroup } from '@angular/forms';

import { AlertService } from '@pwa/service/alert.service';
import { ClipboardService } from '@pwa/service/clipboard.service';
import { HelpComponent } from '@pwa/component/help/help.component';
import { KvsService } from '@pwa/service/kvs.service';
import { ModalController } from '@ionic/angular';
import { MutexInterface } from 'async-mutex';
import { ProxyService } from '@pwa/service/proxy.service';
import { validateProxyAddress } from '@pwa/helper/proxyAddress';

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
        private proxyService: ProxyService
    ) {}

    ngOnInit(): void {
        this.createFormGroup();
    }

    async showHelp(): Promise<void> {
        const modal = await this.modalController.create({
            component: HelpComponent,
            componentProps: {
                url: 'assets/doc/settings.md',
            },
        });
        await modal.present();
    }

    ionViewWillEnter(): void {
        this.mutexReleasePromise = this.kvsService.mutex.acquire();
    }

    async ionViewWillLeave(): Promise<void> {
        if (this.formGroup.get(fields.networkRedirection)?.value && !this.formGroup.get(fields.proxyServer)?.valid) {
            this.alertService.message(
                'Invalid proxy server',
                'The proxy server you specified is invalid. Network redirection will be disabled.'
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
        });

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
                    this.alertService.errorMessage('Unable to connect to proxy');
                    break;

                case 'version_mismatch':
                    this.alertService.proxyVersionMismatchError();
                    break;

                default:
                    this.alertService.message('Success', 'Connection to proxy successful.');
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

    private createFormGroup() {
        this.formGroup = new FormGroup({
            [fields.volume]: new FormControl(this.kvsService.kvs.volume),
            [fields.showStatistics]: new FormControl(this.kvsService.kvs.showStatistics),
            [fields.runHidden]: new FormControl(this.kvsService.kvs.runHidden),
            [fields.clipboardIntegration]: new FormControl(this.kvsService.kvs.clipboardIntegration),
            [fields.networkRedirection]: new FormControl(this.kvsService.kvs.networkRedirection),
            [fields.proxyServer]: new FormControl(this.kvsService.kvs.proxyServer, {
                validators: [validateProxyAddress],
            }),
            [fields.autoLockUI]: new FormControl(this.kvsService.kvs.autoLockUI),
            [fields.enableRemoteInstall]: new FormControl(this.kvsService.kvs.enableRemoteInstall),
            [fields.audioOnStart]: new FormControl(this.kvsService.kvs.enableAudioOnFirstInteraction),
        });

        this.formGroup.get(fields.audioOnStart)?.valueChanges.subscribe(this.onAudioOnStartChange);
    }

    private onAudioOnStartChange = (audioOnStart: boolean) => {
        this.kvsService.kvs.enableAudioOnFirstInteraction = audioOnStart;

        if (!audioOnStart) return;

        this.alertService.message(
            'Enable audio on start',
            `
            Audio will automatically turn on after the first interaction with the application
            (i.e. touch, click or keyboard event). This option will only take effect the next time
            Coudpilot is restarts.
        `
        );
    };

    formGroup!: FormGroup;
    private connectionTestInProgress = false;
    private mutexReleasePromise: Promise<MutexInterface.Releaser> | undefined;
}
