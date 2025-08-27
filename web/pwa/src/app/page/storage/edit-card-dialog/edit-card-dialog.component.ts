import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import { CardSupportLevel } from '@native/cloudpilot_web';
import { StorageCard } from '@pwa/model/StorageCard';
import { StorageCardService } from '@pwa/service/storage-card.service';

export type CardSettings = Pick<StorageCard, 'name' | 'size' | 'dontFsckAutomatically'>;

@Component({
    selector: 'app-edit-card-dialog',
    templateUrl: './edit-card-dialog.component.html',
    styleUrls: ['./edit-card-dialog.component.scss'],
    standalone: false,
})
export class EditCardDialogComponent<T extends CardSettings> implements OnInit {
    constructor(private storageCardService: StorageCardService) {}

    ngOnInit(): void {
        if (!this.card) {
            return;
        }

        this.formGroup.controls.name.setValue(this.card.name);
        this.formGroup.controls.checkAutomatically.setValue(!this.card.dontFsckAutomatically);
    }

    save(): void {
        if (!this.formGroup.valid || !this.formGroup.value.name || !this.card) {
            return;
        }

        this.onSave({
            name: this.formGroup.value.name,
            dontFsckAutomatically: !this.formGroup.value.checkAutomatically,
        });
    }

    onEnter(): void {
        this.save();
    }

    get memoryStickSupported(): boolean {
        return this.cardSupportLevel === CardSupportLevel.sdAndMs;
    }

    private validateNameUnique = (control: AbstractControl<string>): ValidationErrors | null => {
        return this.storageCardService
            .getAllCards()
            .some((card) => card.name === control.value && card.name !== this.card?.name)
            ? { name: 'already taken' }
            : null;
    };

    formGroup = new FormGroup({
        name: new FormControl('', { nonNullable: true, validators: [Validators.required, this.validateNameUnique] }),
        size: new FormControl({ value: 'dummy', disabled: true }),
        checkAutomatically: new FormControl(true, { nonNullable: true }),
    });

    @Input()
    card: T | undefined;

    @Input()
    newCard = false;

    @Input()
    cardSupportLevel = CardSupportLevel.sdAndMs;

    @Input()
    onCancel = () => undefined;

    @Input()
    private onSave: (update: Partial<CardSettings>) => void = () => undefined;
}
