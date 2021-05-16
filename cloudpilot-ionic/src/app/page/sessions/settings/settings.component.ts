import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import { DeviceId } from 'src/app/model/DeviceId';
import { SessionService } from '../../../service/session.service';
import { deviceName } from 'src/app/helper/deviceProperties';

export interface SessionSettings {
    name: string;
    device: DeviceId;
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
        return this.formGroup.get('name')!;
    }

    get formControlHotsyncName(): AbstractControl {
        return this.formGroup.get('hotsyncName')!;
    }

    get forControlDevice(): AbstractControl {
        return this.formGroup.get('device')!;
    }

    get placeholder(): string {
        if (this.formControlHotsyncName.value) return this.formControlName.value;

        return this.session.hotsyncName === undefined ? 'use setting from device' : 'Enter hotsync name';
    }

    get deviceList(): Array<[DeviceId, string]> {
        return this.availableDevices.map((device) => [device, deviceName(device)]);
    }

    ngOnInit() {
        this.createFormGroup();
    }

    save(): void {
        if (this.formGroup.invalid) return;

        this.session.name = this.formControlName.value;

        if (this.formControlHotsyncName.value) {
            this.session.hotsyncName = this.formControlHotsyncName.value;
        } else {
            this.session.hotsyncName = this.session.hotsyncName === undefined ? undefined : '';
        }

        this.session.device = this.forControlDevice.value;

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
            device: new FormControl({ value: this.session.device, disabled: this.availableDevices.length === 1 }),
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

    @Input()
    availableDevices!: Array<DeviceId>;

    formGroup!: FormGroup;
}
