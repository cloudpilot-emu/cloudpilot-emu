import { Component, OnInit } from '@angular/core';
import { FormControl, FormGroup } from '@angular/forms';

import { ClipboardService } from './../../service/clipboard.service';
import { HelpComponent } from 'src/app/component/help/help.component';
import { KvsService } from './../../service/kvs.service';
import { ModalController } from '@ionic/angular';

@Component({
    selector: 'app-settings',
    templateUrl: './settings.page.html',
    styleUrls: ['./settings.page.scss'],
})
export class SettingsPage implements OnInit {
    constructor(
        private modalController: ModalController,
        private kvsService: KvsService,
        public clipboardService: ClipboardService
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
            volume: new FormControl(this.kvsService.kvs.volume),
            showStatistics: new FormControl(this.kvsService.kvs.showStatistics),
            clipboardIntegration: new FormControl(this.kvsService.kvs.clipboardIntegration),
            networkRedirection: new FormControl(this.kvsService.kvs.networkRedirection),
            proxyServer: new FormControl(this.kvsService.kvs.proxyServer),
        });
    }

    formGroup!: FormGroup;
}
