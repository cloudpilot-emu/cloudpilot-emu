import { AbstractControl, FormControl, FormGroup, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import { ModalController } from '@ionic/angular';
import { Session } from './../../../../model/Session';

@Component({
    selector: 'app-session-settings',
    templateUrl: './settings.component.html',
    styleUrls: ['./settings.component.scss'],
})
export class SessionSettingsComponent implements OnInit {
    constructor(public modalControler: ModalController) {}

    ngOnInit() {
        this.createFormGroup();
    }

    get formControlName(): AbstractControl {
        return this.formGroup.get('name')!;
    }

    get formControlHotsyncName(): AbstractControl {
        return this.formGroup.get('hotsyncName')!;
    }

    get placeholder(): string {
        if (this.formControlHotsyncName.value) return this.formControlName.value;

        return this.session.hotsyncName === undefined ? 'use setting from device' : '';
    }

    save(): void {
        if (this.formGroup.invalid) return;

        this.session.name = this.formControlName.value;

        if (this.formControlHotsyncName.value) {
            this.session.hotsyncName = this.formControlHotsyncName.value;
        } else {
            this.session.hotsyncName = this.session.hotsyncName === undefined ? undefined : '';
        }

        this.modalControler.dismiss();
    }

    formKeyDown(e: KeyboardEvent): void {
        if (e.key === 'Enter') this.save();
    }

    private createFormGroup() {
        this.formGroup = new FormGroup({
            name: new FormControl(this.session.name, {
                validators: [Validators.required],
            }),
            hotsyncName: new FormControl(this.session.hotsyncName || ''),
        });
    }

    @Input()
    session!: Session;

    formGroup!: FormGroup;
}
