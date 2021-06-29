import { AbstractControl, FormControl, FormGroup } from '@angular/forms';
import { Component, OnInit } from '@angular/core';

import { AlertService } from './../../service/alert.service';
import { ClipboardService } from './../../service/clipboard.service';
import { HelpComponent } from 'src/app/component/help/help.component';
import { KvsService } from './../../service/kvs.service';
import { ModalController } from '@ionic/angular';
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
        private alertService: AlertService
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

    ionViewWillLeave(): void {
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

        this.kvsService.set({
            volume: this.formGroup.get('volume')!.value,
            showStatistics: this.formGroup.get('showStatistics')!.value,
            clipboardIntegration: this.formGroup.get('clipboardIntegration')?.value,
            networkRedirection: this.formGroup.get('networkRedirection')?.value,
            proxyServer: this.formGroup.get('proxyServer')?.value,
        });
    }

    get showProxyServer(): boolean {
        return this.formGroup.get('networkRedirection')?.value;
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
}
