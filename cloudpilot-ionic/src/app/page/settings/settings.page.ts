import { Component, OnInit } from '@angular/core';
import { FormControl, FormGroup } from '@angular/forms';

import { HelpComponent } from 'src/app/component/help/help.component';
import { KvsService } from './../../service/kvs.service';
import { ModalController } from '@ionic/angular';
import { REVISION } from './../../../revision.gen';

@Component({
    selector: 'app-settings',
    templateUrl: './settings.page.html',
    styleUrls: ['./settings.page.scss'],
})
export class SettingsPage implements OnInit {
    constructor(private modalController: ModalController, private kvsService: KvsService) {}

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

    get version(): string {
        return REVISION;
    }

    ionViewWillLeave(): void {
        this.kvsService.kvs.volume = this.formGroup.get('volume')!.value;
    }

    private createFormGroup() {
        this.formGroup = new FormGroup({
            volume: new FormControl(this.kvsService.kvs.volume),
        });
    }

    formGroup!: FormGroup;
}
