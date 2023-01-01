import { ActionSheetController, IonItemSliding, PopoverController } from '@ionic/angular';
import { Component, EventEmitter, Input, OnChanges, Output, SimpleChanges, ViewChild } from '@angular/core';

import { ContextMenuFileEntryComponent } from './../context-menu-file-entry/context-menu-file-entry.component';
import { FileEntry } from '@common/bridge/Vfs';

@Component({
    selector: 'app-file-entry',
    templateUrl: './file-entry.component.html',
    styleUrls: ['./file-entry.component.scss'],
})
export class FileEntryComponent implements OnChanges {
    constructor(private actionSheetController: ActionSheetController, private popoverController: PopoverController) {}

    ngOnChanges(changes: SimpleChanges): void {
        if (changes['selecting'] && changes['selecting'].currentValue && !changes['selecting'].previousValue) {
            void this.slidingItem?.close();
        }
    }

    // eslint-disable-next-line @typescript-eslint/no-unused-vars
    async onContextmenu(e: MouseEvent): Promise<void> {
        e.stopPropagation();
        e.preventDefault();

        this.interaction.emit();

        const popover = await this.popoverController.create({
            component: ContextMenuFileEntryComponent,
            event: e,
            arrow: false,
            dismissOnSelect: true,
            componentProps: {
                onEdit: () => this.editEntry.emit(this.entry),
                onSave: () => this.saveEntry.emit(this.entry),
                onCopy: () => this.copyEntry.emit(this.entry),
                onCut: () => this.cutEntry.emit(this.entry),
                onDelete: () => this.deleteEntry.emit(this.entry),
            },
        });

        void popover.present();
    }

    get color(): string | undefined {
        if (!this.selecting && this.touched) return 'light';
        if (this.selecting && this.selected) return 'light';

        return undefined;
    }

    async cutOrCopyEntry(): Promise<void> {
        const entryType = this.entry?.isDirectory ? 'directory' : 'file';

        const sheet = await this.actionSheetController.create({
            header: `Do you want to cut or copy?`,
            buttons: [
                {
                    text: `Copy ${entryType}`,
                    handler: () => this.copyEntry.emit(this.entry),
                },
                {
                    text: `Cut ${entryType}`,
                    handler: () => this.cutEntry.emit(this.entry),
                },
            ],
        });

        void sheet.present();
    }

    @ViewChild('slidingItem')
    slidingItem: IonItemSliding | undefined;

    @Input()
    entry: FileEntry | undefined;

    @Input()
    touched = false;

    @Input()
    selecting = false;

    @Input()
    selected = false;

    @Output()
    interaction = new EventEmitter<FileEntry>();

    @Output()
    selectEntry = new EventEmitter<FileEntry>();

    @Output()
    editEntry = new EventEmitter<FileEntry>();

    @Output()
    copyEntry = new EventEmitter<FileEntry>();

    @Output()
    cutEntry = new EventEmitter<FileEntry>();

    @Output()
    deleteEntry = new EventEmitter<FileEntry>();

    @Output()
    saveEntry = new EventEmitter<FileEntry>();
}
