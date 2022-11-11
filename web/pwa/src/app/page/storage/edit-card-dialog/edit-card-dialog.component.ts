import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import { StorageCard } from '@pwa/model/StorageCard';
import { StorageCardService } from '@pwa/service/storage-card.service';

@Component({
    selector: 'app-edit-card-dialog',
    templateUrl: './edit-card-dialog.component.html',
    styleUrls: ['./edit-card-dialog.component.scss'],
})
export class EditCardDialogComponent implements OnInit {
    @Input()
    onCancel = () => undefined;

    @Input()
    private onSave = (card: StorageCard) => undefined;

    constructor(private storageCardService: StorageCardService) {}

    ngOnInit(): void {
        if (!this.card) {
            return;
        }

        this.formGroup.get('name')?.setValue(this.card.name);
    }

    save(): void {
        if (!this.formGroup.valid || !this.formGroup.value.name || !this.card) {
            return;
        }

        this.onSave({ ...this.card, name: this.formGroup.value.name });
    }

    onEnter(): void {
        this.save();
    }

    private validateNameUnique = (control: AbstractControl<string>): ValidationErrors | null => {
        return this.storageCardService.getAllCards().some((card) => card.name === control.value)
            ? { name: 'already taken' }
            : null;
    };

    formGroup = new FormGroup({
        name: new FormControl('', { nonNullable: true, validators: [Validators.required, this.validateNameUnique] }),
        size: new FormControl('dummy'),
    });

    @Input()
    card: StorageCard | undefined;
}
