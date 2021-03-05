import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import { ModalController } from '@ionic/angular';
import { Session } from '../../../model/Session';
import { SessionService } from '../../../service/session.service';

export interface SessionSettings {
    name: string;
    hotsyncName?: string;
}

@Component({
    selector: 'app-session-settings',
    templateUrl: './settings.component.html',
    styleUrls: ['./settings.component.scss'],
})
export class SessionSettingsComponent implements OnInit {
    constructor(private sessionService: SessionService) {}

    get formControlName(): AbstractControl {
        // tslint:disable-next-line: no-non-null-assertion
        return this.formGroup.get('name')!;
    }

    get formControlHotsyncName(): AbstractControl {
        // tslint:disable-next-line: no-non-null-assertion
        return this.formGroup.get('hotsyncName')!;
    }

    get placeholder(): string {
        if (this.formControlHotsyncName.value) return this.formControlName.value;

        return this.session.hotsyncName === undefined ? 'use setting from device' : '';
    }

    ngOnInit() {
        this.createFormGroup();
    }

    save(): void {
        if (this.formGroup.invalid) return;

        this.session.name = this.formControlName.value;

        /*
        if (this.formControlHotsyncName.value) {
            this.session.hotsyncName = this.formControlHotsyncName.value;
        } else {
            this.session.hotsyncName = this.session.hotsyncName === undefined ? undefined : '';
        }
        */

        this.onSave();
    }

    formKeyDown(e: KeyboardEvent): void {
        if (e.key === 'Enter') this.save();
    }

    private createFormGroup() {
        this.formGroup = new FormGroup({
            name: new FormControl(this.session.name, {
                validators: [Validators.required, this.validateNameUnique],
            }),
            hotsyncName: new FormControl(this.session.hotsyncName || ''),
        });
    }

    private validateNameUnique = (control: AbstractControl): ValidationErrors | null => {
        return control.value !== this.session.name &&
            this.sessionService.getSessions().some((s) => s.name === control.value)
            ? { name: 'already taken' }
            : null;
    };

    @Input()
    onSave: () => void = () => undefined;

    @Input()
    onCancel: () => void = () => undefined;

    @Input()
    session!: SessionSettings;

    formGroup!: FormGroup;
}
