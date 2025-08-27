import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input } from '@angular/core';
import { StorageCardService, calculateNewCardSizeBytes } from '@pwa//service/storage-card.service';

import { NewCardSize } from '@pwa/service/storage-card.service';

const SIZES = [
    NewCardSize.mb4,
    NewCardSize.mb8,
    NewCardSize.mb16,
    NewCardSize.mb32,
    NewCardSize.mb64,
    NewCardSize.mb128,
];

@Component({
    selector: 'app-new-card-dialog',
    templateUrl: './new-card-dialog.component.html',
    styleUrls: ['./new-card-dialog.component.scss'],
    standalone: false,
})
export class NewCardDialogComponent {
    constructor(private storageCardService: StorageCardService) {}

    get sizes(): Array<NewCardSize> {
        return SIZES;
    }

    sizeBytes(newCardSize: NewCardSize): number {
        return calculateNewCardSizeBytes(newCardSize);
    }

    create(): void {
        if (
            !this.formGroup.valid ||
            this.formGroup.value.name === undefined ||
            this.formGroup.value.size === undefined
        ) {
            return;
        }

        this.onCreate(this.formGroup.value.name, this.formGroup.value.size, !this.formGroup.value.checkAutomatically!);
    }

    onEnter(): void {
        this.create();
    }

    private validateNameUnique = (control: AbstractControl<string>): ValidationErrors | null => {
        return this.storageCardService.getAllCards().some((card) => card.name === control.value)
            ? { name: 'already taken' }
            : null;
    };

    @Input()
    private onCreate: (name: string, size: NewCardSize, dontFsckAutomatically: boolean) => void = () => undefined;

    @Input()
    onCancel = () => undefined;

    formGroup = new FormGroup({
        name: new FormControl('', { nonNullable: true, validators: [Validators.required, this.validateNameUnique] }),
        size: new FormControl(NewCardSize.mb32, { nonNullable: true }),
        checkAutomatically: new FormControl(true, { nonNullable: true }),
    });
}
