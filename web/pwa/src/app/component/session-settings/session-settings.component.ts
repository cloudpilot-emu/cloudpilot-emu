import { Component, Input, OnInit } from '@angular/core';
import { AbstractControl, UntypedFormControl, UntypedFormGroup, ValidationErrors, Validators } from '@angular/forms';
import { cpuClock, deviceName } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';

import { memoize } from '@pwa/helper/memoize';
import {
    MAX_HOST_LOAD_DEFAULT,
    SessionSettings,
    TARGET_MIPS_DEFAULT,
    WARN_SLOWDOWN_THRESHOLD_DEFAULT,
} from '@pwa/model/Session';
import { AlertService } from '@pwa/service/alert.service';
import { FileService } from '@pwa/service/file.service';
import { SessionService } from '@pwa/service/session.service';

@Component({
    selector: 'app-session-settings',
    templateUrl: './session-settings.component.html',
    styleUrls: ['./session-settings.component.scss'],
    standalone: false,
})
export class SessionSettingsComponent implements OnInit {
    constructor(
        private sessionService: SessionService,
        private alertService: AlertService,
        private fileService: FileService,
    ) {}

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

    get formControlTargetMips(): AbstractControl {
        return this.formGroup.get('targetMips')!;
    }

    get formControlWarnSlowdownThreshold(): AbstractControl {
        return this.formGroup.get('warnSlowdownThreshold')!;
    }

    get formControlMaxHostLoad(): AbstractControl {
        return this.formGroup.get('maxHostLoad')!;
    }

    get formControlDisableAudioEmulation(): AbstractControl {
        return this.formGroup.get('disableAudioEmulation')!;
    }

    get formControlNand(): AbstractControl {
        return this.formGroup.get('nand')!;
    }

    get showHotsyncNameInput(): boolean {
        return this.formControlManageHotsyncName.value;
    }

    get isCloudpilot(): boolean {
        return this.settings.engine === 'cloudpilot';
    }

    get isUarm(): boolean {
        return this.settings.engine === 'uarm';
    }

    private memoizedDevicelist = memoize<Array<DeviceId>, Array<[DeviceId, string]>>((x) =>
        x.map((device) => [device, deviceName(device)]),
    );

    get deviceList(): Array<[DeviceId, string]> {
        return this.memoizedDevicelist(this.availableDevices);
    }

    ngOnInit() {
        this.createFormGroup();
    }

    save(): void {
        if (this.formGroup.invalid) return;

        this.settings.name = this.formControlName.value;
        this.settings.deviceOrientation = this.formControlOrientation.value;

        this.saveCloudpilot();
        this.saveUarm();

        this.onSave(this.formControlDevice.value, this.nand);
    }

    onEnter(): void {
        this.save();
    }

    get speedValue(): number {
        const speed = this.speedTransient ?? this.formControlSpeed.value ?? 0;

        return speed >= 0 ? 1 + speed : 1 / (1 - speed);
    }

    get speedLabel(): string {
        return (
            'Clock: ' +
            ((this.speedTransient ?? this.formControlSpeed.value) === 0
                ? 'default'
                : `${Math.round(this.speedValue * cpuClock(this.formControlDevice.value))}MHz`)
        );
    }

    get targetMipsValue(): number {
        return this.targetMipsTransient ?? this.formControlTargetMips.value ?? TARGET_MIPS_DEFAULT;
    }

    get targetMipsLabel(): string {
        return `Target speed: ${this.targetMipsValue} MIPS`;
    }

    get warnSlowdownThresholdValue(): number {
        return (
            this.warnSlowdownThresholdTransient ??
            this.formControlWarnSlowdownThreshold.value ??
            WARN_SLOWDOWN_THRESHOLD_DEFAULT * 100
        );
    }

    get warnSlowdownThresholdLabel(): string {
        return `Minimum speed: ${this.warnSlowdownThresholdValue}%`;
    }

    get maxHostLoadValue(): number {
        return this.maxHostLoadTransient ?? this.formControlMaxHostLoad.value ?? MAX_HOST_LOAD_DEFAULT * 100;
    }

    get maxHostLoadLabel(): string {
        return `Load limit: ${this.maxHostLoadValue}%`;
    }

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    onSpeedDrag(e: any): void {
        this.speedTransient = e.detail?.value;
    }

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    onTargetMipsDrag(e: any): void {
        this.targetMipsTransient = e.detail?.value;
    }

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    onWarnSlowdownThresholdDrag(e: any): void {
        this.warnSlowdownThresholdTransient = e.detail?.value;
    }

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    onMaxHostLoadDrag(e: any): void {
        this.maxHostLoadTransient = e.detail?.value;
    }

    onNandClick(): void {
        this.fileService.openFile(async (file) => {
            let content: Uint8Array;
            try {
                content = await file.getContent();
            } catch (e) {
                console.warn(e);

                await this.alertService.errorMessage(`Unable to open ${file.name}.`);
                return;
            }

            if (content.length !== this.selectNandSize) {
                void this.alertService.errorMessage(`${file.name} is not a valid NAND image.`);
                return;
            }

            this.nand = content;
            this.formControlNand.setValue(file.name);
        });
    }

    onNandClearClick(): void {
        this.nand = undefined;
        this.formControlNand.setValue(undefined);
    }

    private createFormGroup() {
        this.formGroup = new UntypedFormGroup({
            name: new UntypedFormControl(this.settings.name, {
                validators: [Validators.required, this.validateNameUnique],
            }),
            device: new UntypedFormControl({
                value: this.device,
                disabled: this.availableDevices.length === 1,
            }),
            orientation: new UntypedFormControl(this.settings.deviceOrientation),
            manageHotsyncName: new UntypedFormControl(
                this.settings.engine === 'cloudpilot' ? !this.settings.dontManageHotsyncName : false,
            ),
            hotsyncName: new UntypedFormControl(this.settings.engine === 'cloudpilot' ? this.settings.hotsyncName : ''),
            speed: new UntypedFormControl(
                this.settings.engine === 'cloudpilot'
                    ? this.settings.speed >= 1
                        ? this.settings.speed - 1
                        : 1 - 1 / this.settings.speed
                    : 0,
            ),
            targetMips: new UntypedFormControl(this.settings.engine === 'uarm' ? this.settings.targetMips : 0),
            warnSlowdownThreshold: new UntypedFormControl(
                this.settings.engine === 'uarm' ? Math.round(this.settings.warnSlowdownThreshold * 100) : 0,
            ),
            maxHostLoad: new UntypedFormControl(
                this.settings.engine === 'uarm' ? Math.round(100 * this.settings.maxHostLoad) : 0,
            ),
            disableAudioEmulation: new UntypedFormControl(
                this.settings.engine === 'uarm' ? this.settings.disableAudio : false,
            ),
            nand: new UntypedFormControl(),
        });
    }

    private validateNameUnique = (control: AbstractControl): ValidationErrors | null => {
        return control.value !== this.settings.name &&
            this.sessionService.sessions().some((s) => s.name === control.value)
            ? { name: 'already taken' }
            : null;
    };

    private saveCloudpilot(): void {
        if (this.settings.engine !== 'cloudpilot') return;

        if (this.formControlManageHotsyncName.value) {
            if (this.settings.dontManageHotsyncName) {
                this.settings.dontManageHotsyncName = false;
                this.settings.hotsyncName = this.formControlHotsyncName.value || '';

                void this.alertService.message(
                    'Reset required',
                    'Please reset the virtual device in order to make sure that the hotsync name is synced properly again.',
                );
            } else {
                this.settings.hotsyncName = this.formControlHotsyncName.value || this.settings.hotsyncName;
            }
        } else {
            this.settings.dontManageHotsyncName = true;
        }

        this.settings.speed = this.speedValue;
    }

    private saveUarm(): void {
        if (this.settings.engine !== 'uarm') return;

        this.settings.targetMips = this.targetMipsValue;
        this.settings.warnSlowdownThreshold = this.warnSlowdownThresholdValue / 100;
        this.settings.maxHostLoad = this.maxHostLoadValue / 100;
        this.settings.disableAudio = this.formControlDisableAudioEmulation.value;
    }

    @Input()
    onSave: (device: DeviceId, nand?: Uint8Array) => void = () => undefined;

    @Input()
    onCancel: () => void = () => undefined;

    @Input()
    settings!: SessionSettings;

    @Input()
    availableDevices!: Array<DeviceId>;

    @Input()
    device!: DeviceId;

    @Input()
    selectNandSize: number | undefined;

    formGroup!: UntypedFormGroup;

    speedTransient: number | undefined;
    targetMipsTransient: number | undefined;
    warnSlowdownThresholdTransient: number | undefined;
    maxHostLoadTransient: number | undefined;

    nand: Uint8Array | undefined;

    readonly orientations = [
        [DeviceOrientation.portrait, 'Portrait'],
        [DeviceOrientation.landscape90, 'Landscape 90°'],
        [DeviceOrientation.landscape270, 'Landscape 270°'],
        [DeviceOrientation.portrait180, 'Upside down'],
    ];
}
export { SessionSettings };
