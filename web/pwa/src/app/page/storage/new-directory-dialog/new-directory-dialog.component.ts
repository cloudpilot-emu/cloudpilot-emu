import { Component, Input } from '@angular/core';
import { AbstractControl, FormControl, FormGroup, ValidationErrors, Validators } from '@angular/forms';
import { ModalController } from '@ionic/angular';

import { VfsService } from '@pwa/service/vfs.service';

@Component({
    selector: 'app-new-directory-dialog',
    templateUrl: './new-directory-dialog.component.html',
    styleUrls: ['./new-directory-dialog.component.scss'],
    standalone: false,
})
export class NewDirectoryDialogComponent {
    constructor(
        private vfsService: VfsService,
        private modalController: ModalController,
    ) {}

    onCreateClick(): void {
        this.createDir();
    }

    onEnter(): void {
        this.createDir();
    }

    onCancelClick(): void {
        void this.modalController.dismiss();
    }

    private createDir(): void {
        if (!this.formGroup.controls.name.valid) return;

        this.onCreate(this.formGroup.controls.name.value);
        void this.modalController.dismiss();
    }

    private validateNameUnique = (control: AbstractControl<string>): ValidationErrors | null => {
        if (!this.parentPath) return null;

        return this.vfsService
            .readdir(this.parentPath)
            .some((entry) => entry.name.toLocaleLowerCase() === control.value.toLocaleLowerCase())
            ? { name: 'already taken' }
            : null;
    };

    private validateNameValid = (control: AbstractControl<string>): ValidationErrors | null => {
        return this.vfsService.isFilenameValid(control.value) ? null : { name: 'invalid file name' };
    };

    @Input()
    parentPath: string | undefined;

    @Input()
    onCreate: (name: string) => void = () => undefined;

    formGroup = new FormGroup({
        name: new FormControl('', {
            nonNullable: true,
            validators: [Validators.required, this.validateNameUnique, this.validateNameValid],
        }),
    });
}
