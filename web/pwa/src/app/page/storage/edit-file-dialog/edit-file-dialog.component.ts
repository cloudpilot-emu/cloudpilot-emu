import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import { FileEntry } from '@common/bridge/Vfs';
import { ModalController } from '@ionic/angular';
import { VfsService } from '@pwa//service/vfs.service';
import { debounce } from '@pwa/helper/debounce';

@Component({
    selector: 'app-edit-file-dialog',
    templateUrl: './edit-file-dialog.component.html',
    styleUrls: ['./edit-file-dialog.component.scss'],
    standalone: false
})
export class EditFileDialogComponent implements OnInit {
    constructor(
        private modalController: ModalController,
        private vfsService: VfsService,
    ) {}

    ngOnInit(): void {
        if (!this.entry) return;

        this.formGroup.controls.name.setValue(this.entry.name);
        this.formGroup.controls.attrHidden.setValue(this.entry.attributes.hidden);
        this.formGroup.controls.attrReadonly.setValue(this.entry.attributes.readonly);
        this.formGroup.controls.attrSystem.setValue(this.entry.attributes.system);
    }

    get subjectName(): string {
        return this.entry?.isDirectory ? 'Directory' : 'File';
    }

    @debounce()
    async onSaveClick(): Promise<void> {
        await this.save();
    }

    onCancelClick(): void {
        void this.modalController.dismiss();
    }

    @debounce()
    async onEnter(): Promise<void> {
        await this.save();
    }

    private async save(): Promise<void> {
        if (!this.entry) return;
        if (!this.formGroup.valid) return;

        await this.vfsService.updateFileEntry(this.entry.path, {
            name: this.formGroup.controls.name.value ?? '',
            attributes: {
                hidden: !!this.formGroup.controls.attrHidden.value,
                readonly: !!this.formGroup.controls.attrReadonly.value,
                system: !!this.formGroup.controls.attrSystem.value,
            },
        });

        this.onSave();
        void this.modalController.dismiss();
    }

    private validateNameUnique = (control: AbstractControl<string>): ValidationErrors | null => {
        if (!this.entry) return null;

        return this.vfsService
            .readdir(this.vfsService.dirname(this.entry.path))
            .some(
                (entry) =>
                    entry.name.toLocaleLowerCase() === control.value.toLocaleLowerCase() &&
                    entry.name.toLocaleLowerCase() !== this.entry?.name?.toLocaleLowerCase(),
            )
            ? { name: 'already taken' }
            : null;
    };

    private validateNameValid = (control: AbstractControl<string>): ValidationErrors | null => {
        return this.vfsService.isFilenameValid(control.value) ? null : { name: 'invalid file name' };
    };

    @Input()
    entry: FileEntry | undefined;

    onSave: () => void = () => undefined;

    formGroup = new FormGroup({
        name: new FormControl('', {
            nonNullable: true,
            validators: [Validators.required, this.validateNameUnique, this.validateNameValid],
        }),
        attrReadonly: new FormControl(false, { nonNullable: true }),
        attrHidden: new FormControl(false, { nonNullable: true }),
        attrSystem: new FormControl(false, { nonNullable: true }),
    });
}
