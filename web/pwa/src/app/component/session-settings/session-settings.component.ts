import { AbstractControl, UntypedFormControl, UntypedFormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';
import { cpuClock, deviceName } from '@common/helper/deviceProperties';

import { AlertService } from '@pwa/service/alert.service';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { SessionService } from '@pwa/service/session.service';

export interface SessionSettings {
    name: string;
    device: DeviceId;
    hotsyncName?: string;
    dontManageHotsyncName?: boolean;
    speed?: number;
    deviceOrientation?: DeviceOrientation;
}

@Component({
    selector: 'app-session-settings',
    templateUrl: './session-settings.component.html',
    styleUrls: ['./session-settings.component.scss'],
})
export class SessionSettingsComponent implements OnInit {
    constructor(private sessionService: SessionService, private alertService: AlertService) {}

    get formControlName(): AbstractControl {
        return this.formGroup.get('name')!;
    }

    get formControlHotsyncName(): AbstractControl {
        return this.formGroup.get('hotsyncName')!;
    }

    get formControlDevice(): AbstractControl {
        return this.formGroup.get('device')!;
    }

    get formControlManageHotsyncName(): AbstractControl {
        return this.formGroup.get('manageHotsyncName')!;
    }

    get formControlSpeed(): AbstractControl {
        return this.formGroup.get('speed')!;
    }

    get formControlOrientation(): AbstractControl {
        return this.formGroup.get('orientation')!;
    }

    get showHotsyncNameInput(): boolean {
        return this.formControlManageHotsyncName.value;
    }

    get placeholder(): string {
        if (this.formControlHotsyncName.value) return this.formControlName.value;

        return this.session.hotsyncName === undefined && !this.session.dontManageHotsyncName
            ? 'use setting from device'
            : 'Enter hotsync name';
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

        if (this.formControlManageHotsyncName.value) {
            if (this.session.dontManageHotsyncName) {
                this.session.dontManageHotsyncName = false;
                this.session.hotsyncName = this.formControlHotsyncName.value || '';

                this.alertService.message(
                    'Reset required',
                    'Please reset the virtual device in order to make sure that the hotsync name is synced properly again.'
                );
            } else {
                this.session.hotsyncName =
                    this.formControlHotsyncName.value || (this.session.hotsyncName === undefined ? undefined : '');
            }
        } else {
            this.session.dontManageHotsyncName = true;
        }

        this.session.device = this.formControlDevice.value;
        this.session.speed = this.speedValue;
        this.session.deviceOrientation = this.formControlOrientation.value;

        this.onSave();
    }

    onEnter(): void {
        this.save();
    }

    get speedValue(): number {
        const speed = this.formControlSpeed.value || 0;

        return speed >= 0 ? 1 + speed : 1 / (1 - speed);
    }

    get speedLabel(): string {
        return (
            'Clock: ' +
            (this.formControlSpeed.value === 0
                ? 'default'
                : `${Math.round(this.speedValue * cpuClock(this.formControlDevice.value))}MHz`)
        );
    }

    private createFormGroup() {
        this.formGroup = new UntypedFormGroup({
            name: new UntypedFormControl(this.session.name, {
                validators: [Validators.required, this.validateNameUnique],
            }),
            manageHotsyncName: new UntypedFormControl(!this.session.dontManageHotsyncName),
            hotsyncName: new UntypedFormControl(this.session.hotsyncName || ''),
            device: new UntypedFormControl({ value: this.session.device, disabled: this.availableDevices.length === 1 }),
            speed: new UntypedFormControl(
                (this.session.speed || 1) >= 1 ? (this.session.speed || 1) - 1 : 1 - 1 / this.session.speed!
            ),
            orientation: new UntypedFormControl(this.session.deviceOrientation || DeviceOrientation.portrait),
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

    formGroup!: UntypedFormGroup;

    readonly orientations = [
        [DeviceOrientation.portrait, 'Portrait'],
        [DeviceOrientation.landscape90, 'Landscape 90°'],
        [DeviceOrientation.landscape270, 'Landscape 270°'],
        [DeviceOrientation.portrait180, 'Upside down'],
    ];
}
