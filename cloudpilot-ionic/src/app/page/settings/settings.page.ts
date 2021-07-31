import { Component, OnInit } from '@angular/core';
import { FormControl, FormGroup } from '@angular/forms';
import { LoadingController, ModalController } from '@ionic/angular';

import { AlertService } from './../../service/alert.service';
import { ClipboardService } from './../../service/clipboard.service';
import { HelpComponent } from 'src/app/component/help/help.component';
import { KvsService } from './../../service/kvs.service';
import { MutexInterface } from 'async-mutex';
import { ProxyService } from './../../service/proxy.service';
import { validateProxyAddress } from 'src/app/helper/proxyAddress';

const enum fields {
    volume = 'volume',
    showStatistics = 'showStatistics',
    clipboardIntegration = 'clipboardIntegration',
    networkRedirection = 'networkRedirection',
    proxyServer = 'proxyServer',
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
        private loadingController: LoadingController
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
            volume: this.formGroup.get('volume')!.value,
            showStatistics: this.formGroup.get('showStatistics')!.value,
            clipboardIntegration: this.formGroup.get('clipboardIntegration')?.value,
            networkRedirection: this.formGroup.get('networkRedirection')?.value,
            proxyServer: this.formGroup.get('proxyServer')?.value,
        });

        if (this.mutexReleasePromise) {
            (await this.mutexReleasePromise)();
        }
    }

    async testProxyConnection(): Promise<void> {
        if (this.connectionTestInProgress) return;

        const loader = await this.loadingController.create();
        await loader.present();

        this.connectionTestInProgress = true;

        try {
            const handshakeResult = await this.proxyService.handshake(this.formGroup.get(fields.proxyServer)?.value);

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
            loader.dismiss();
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
            [fields.clipboardIntegration]: new FormControl(this.kvsService.kvs.clipboardIntegration),
            [fields.networkRedirection]: new FormControl(this.kvsService.kvs.networkRedirection),
            [fields.proxyServer]: new FormControl(this.kvsService.kvs.proxyServer, {
                validators: [validateProxyAddress],
            }),
        });
    }

    formGroup!: FormGroup;
    private connectionTestInProgress = false;
    private mutexReleasePromise: Promise<MutexInterface.Releaser> | undefined;
}
