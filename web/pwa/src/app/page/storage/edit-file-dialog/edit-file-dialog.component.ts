import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { Component, Input, OnInit } from '@angular/core';

import { FileEntry } from '@common/bridge/Vfs';
import { ModalController } from '@ionic/angular';
import { VfsService } from './../../../service/vfs.service';

@Component({
    selector: 'app-edit-file-dialog',
    templateUrl: './edit-file-dialog.component.html',
    styleUrls: ['./edit-file-dialog.component.scss'],
})
export class EditFileDialogComponent implements OnInit {
    constructor(private modalController: ModalController, private vfsService: VfsService) {}

    ngOnInit(): void {
        if (!this.entry) return;

        this.formGroup.controls.name.setValue(this.entry.name);
        this.formGroup.controls.attrHidden.setValue(this.entry.attributes.hidden);
        this.formGroup.controls.attrReadonly.setValue(this.entry.attributes.readonly);
        this.formGroup.controls.attrSystem.setValue(this.entry.attributes.system);

        void this.vfsService
            .readdir(this.vfsService.dirname(this.entry.path))
            .then((entries) => entries && (this.existingNames = entries.map((e) => e.name)));
    }

    get subjectName(): string {
        return this.entry?.isDirectory ? 'Directory' : 'File';
    }

    onSaveClick(): void {
        void this.save();
    }

    onCancelClick(): void {
        void this.modalController.dismiss();
    }

    onEnter(): void {
        void this.save();
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
        return this.existingNames.some(
            (name) =>
                name.toLocaleLowerCase() === control.value.toLocaleLowerCase() &&
                name.toLocaleLowerCase() !== this.entry?.name?.toLocaleLowerCase()
        )
            ? { name: 'already taken' }
            : null;
    };

    @Input()
    entry: FileEntry | undefined;

    onSave: () => void = () => undefined;

    private existingNames: Array<string> = [];

    formGroup = new FormGroup({
        name: new FormControl('', { nonNullable: true, validators: [Validators.required, this.validateNameUnique] }),
        attrReadonly: new FormControl(false, { nonNullable: true }),
        attrHidden: new FormControl(false, { nonNullable: true }),
        attrSystem: new FormControl(false, { nonNullable: true }),
    });
}
