import { Component, Input, OnInit } from '@angular/core';

import { FileEntry } from '@common/bridge/Vfs';
import { StorageCard } from '@pwa/model/StorageCard';
import { VfsService } from '@pwa/service/vfs.service';

function entrySortFunction(e1: FileEntry, e2: FileEntry): number {
    if (e1.isDirectory && !e2.isDirectory) return -1;
    if (!e1.isDirectory && e2.isDirectory) return 1;

    return e1.name.localeCompare(e2.name);
}

@Component({
    selector: 'app-subpage-directory',
    templateUrl: './subpage-directory.component.html',
    styleUrls: ['./subpage-directory.component.scss'],
})
export class SubpageDirectoryComponent implements OnInit {
    constructor(private vfsService: VfsService) {}

    showHelp(): void {}

    get title(): string {
        switch (this.path) {
            case '/':
                return this.card?.name ?? '';

            case undefined:
                return '';

            default:
                return this.vfsService.splitPath(this.path)[0] ?? '';
        }
    }

    ngOnInit(): void {
        this.updateEntries();
    }

    trackEntryBy(index: number, entry: FileEntry) {
        return entry.name;
    }

    onSelect(entry: FileEntry): void {
        if (entry.isDirectory) this.onNavigate(entry.path);
    }

    private updateEntries(): void {
        if (this.path === undefined) {
            this.entries = [];
            return;
        }

        this.vfsService
            .readdir(this.path)
            .then((entries) => (this.entries = entries ? [...entries].sort(entrySortFunction) : []));
    }

    @Input()
    card: StorageCard | undefined;

    @Input()
    path: string | undefined;

    @Input()
    onNavigate: (path: string) => void = () => undefined;

    entries: Array<FileEntry> | undefined;

    lastEntryTouched: string | undefined = undefined;
}
