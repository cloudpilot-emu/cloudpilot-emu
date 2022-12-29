import { Component, EventEmitter, Input, Output } from '@angular/core';

import { FileEntry } from '@common/bridge/Vfs';

@Component({
    selector: 'app-file-entry',
    templateUrl: './file-entry.component.html',
    styleUrls: ['./file-entry.component.scss'],
})
export class FileEntryComponent {
    constructor() {}

    // eslint-disable-next-line @typescript-eslint/no-unused-vars
    async onContextmenu(e: MouseEvent): Promise<void> {}

    get color(): string | undefined {
        if (this.touched) return 'light';

        return undefined;
    }
    @Input()
    entry: FileEntry | undefined;

    @Input()
    touched = false;

    @Output()
    interaction = new EventEmitter<FileEntry>();

    @Output()
    selectEntry = new EventEmitter<FileEntry>();

    @Output()
    edit = new EventEmitter<FileEntry>();

    @Output()
    check = new EventEmitter<FileEntry>();

    @Output()
    delete = new EventEmitter<FileEntry>();

    @Output()
    save = new EventEmitter<FileEntry>();
}
