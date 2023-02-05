import { ActionSheetController, Config, ModalController, PopoverController } from '@ionic/angular';
import { ChangeDetectionStrategy, ChangeDetectorRef, Component, DoCheck, Input } from '@angular/core';

import { AlertService } from '@pwa/service/alert.service';
import { ContextMenuBreadcrumbComponent } from './../context-menu-breadcrumb/context-menu-breadcrumb.component';
import { ContextMenuDirectoryComponent } from './../context-menu-directory/context-menu-directory.component';
import { EditFileDialogComponent } from './../edit-file-dialog/edit-file-dialog.component';
import { FileEntry } from '@common/bridge/Vfs';
import { StorageCard } from '@pwa/model/StorageCard';
import { VfsService } from '@pwa/service/vfs.service';
import { changeDetector } from '@pwa/helper/changeDetect';
import { debounce } from '@pwa/helper/debounce';
import { memoize } from '@pwa/helper/memoize';

function entrySortFunction(e1: FileEntry, e2: FileEntry): number {
    if (e1.isDirectory && !e2.isDirectory) return -1;
    if (!e1.isDirectory && e2.isDirectory) return 1;

    return e1.name.localeCompare(e2.name);
}

let BREADCRUMB_TRIGGER_INDEX = 0;

@Component({
    selector: 'app-subpage-directory',
    templateUrl: './subpage-directory.component.html',
    styleUrls: ['./subpage-directory.component.scss'],
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class SubpageDirectoryComponent implements DoCheck {
    constructor(
        private vfsService: VfsService,
        private popoverController: PopoverController,
        private config: Config,
        private alertService: AlertService,
        private actionSheetController: ActionSheetController,
        private modalController: ModalController,
        private cd: ChangeDetectorRef
    ) {
        this.breadcrumbTriggerId = `breadcrumb-trigger-${BREADCRUMB_TRIGGER_INDEX++}`;

        this.checkEntries = changeDetector(cd, undefined, () =>
            this.path !== undefined ? vfsService.readdir(this.path) : undefined
        );
    }

    ngDoCheck(): void {
        this.checkEntries();
    }

    get title(): string {
        switch (this.path) {
            case '/':
                return this.card?.name ?? '';

            case undefined:
                return '';

            default: {
                const splitPath = this.vfsService.splitPath(this.path);

                return splitPath[splitPath.length - 1] ?? '';
            }
        }
    }

    get bytesFree(): number {
        return this.vfsService.getBytesFree();
    }

    get bytesTotal(): number {
        return this.vfsService.getBytesTotal();
    }

    get bytesInFiles(): number {
        return this.bytesInFilesMemoized(this.entries);
    }

    get isRoot(): boolean {
        return this.path === '/';
    }

    get fileCount(): number {
        return this.fileCountMemoized(this.entries);
    }

    get entries(): Array<FileEntry> {
        return this.path !== undefined ? [...this.vfsService.readdir(this.path)].sort(entrySortFunction) : [];
    }

    trackEntryBy(index: number, entry: FileEntry) {
        return entry.name;
    }

    onSelect(entry: FileEntry): void {
        if (this.mode === 'browse') {
            if (entry.isDirectory) this.onNavigate(entry.path);
        } else {
            if (this.selection?.has(entry.name)) this.selection.delete(entry.name);
            else this.selection.add(entry.name);
        }
    }

    async onEditEntry(entry: FileEntry): Promise<void> {
        const modal = await this.modalController.create({
            component: EditFileDialogComponent,
            backdropDismiss: false,
            componentProps: {
                entry,
            },
        });

        await modal.present();
    }

    onSaveEntry(entry: FileEntry): void {
        void this.alertService.message('Not implemented', `Save ${entry.name}: not implemented.`);
    }

    onCopyEntry(entry: FileEntry): void {
        void this.alertService.message('Not implemented', `Copy ${entry.name}: not implemented.`);
    }

    onCutEntry(entry: FileEntry): void {
        void this.alertService.message('Not implemented', `Cut ${entry.name}: not implemented.`);
    }

    onDeleteEntry(entry: FileEntry): void {
        void this.alertService.message('Not implemented', `Delete ${entry.name}: not implemented.`);
    }

    onAddFiles(): void {
        void this.alertService.message('Not implemented', 'Add files: not implemented.');
    }

    onCreateDirectory(): void {
        void this.alertService.message('Not implemented', 'Create directory: not implemented.');
    }

    onExtractZipfile(): void {
        void this.alertService.message('Not implemented', 'Extract zipfile: not implemented.');
    }

    onPaste(): void {
        void this.alertService.message('Not implemented', 'Paste: not implemented.');
    }

    onHelp(): void {
        void this.alertService.message('Not implemented', 'Help: not implemented.');
    }

    onStartSelection(): void {
        if (this.entries?.length === 0) return;

        this.selection.clear();
        this.mode = 'select';
        this.cd.markForCheck();
    }

    @debounce()
    async onSelectionDone(): Promise<void> {
        if (!this.entries) return;

        const selectionAsArray = Array.from(this.selection);
        const entryMap = new Map(this.entries.map((entry) => [entry.name, entry]));
        const filesOnly = selectionAsArray.every((name) => !entryMap.get(name)?.isDirectory);
        const directoriesOnly = selectionAsArray.every((name) => entryMap.get(name)?.isDirectory);
        let subject = `${this.selection.size} items`;

        if (filesOnly) subject = this.selection.size === 1 ? 'file' : `${this.selection.size} files`;
        if (directoriesOnly) subject = this.selection.size === 1 ? 'directory' : `${this.selection.size} directories`;

        if (this.selection.size !== 0) {
            const sheet = await this.actionSheetController.create({
                header: `What do you want to do?`,
                buttons: [
                    {
                        text: `Copy ${subject}`,
                        handler: this.onCopySelection.bind(this),
                    },
                    {
                        text: `Cut ${subject}`,
                        handler: this.onCutSelection.bind(this),
                    },
                    {
                        text: `Save ${subject} as zip`,
                        handler: this.onSaveZip.bind(this),
                    },
                    {
                        text: `Delete ${subject}`,
                        handler: this.onDeleteSelection.bind(this),
                    },
                    { text: 'Cancel' },
                ],
            });

            await sheet.present();
            await sheet.onWillDismiss();
        }

        this.mode = 'browse';
        this.cd.markForCheck();
    }

    onSelectAll(): void {
        this.entries?.forEach((entry) => this.selection.add(entry.name));
    }

    onCopySelection(): void {
        void this.alertService.message('Not implemented', 'Copy selection: not implemented.');
    }

    onCutSelection(): void {
        void this.alertService.message('Not implemented', 'Cut selection: not implemented.');
    }

    onSaveZip(): void {
        void this.alertService.message('Not implemented', 'Save zip: not implemented.');
    }

    onDeleteSelection(): void {
        void this.alertService.message('Not implemented', 'Delete selection: not implemented.');
    }

    async openBreadcrumbMenu(e: MouseEvent): Promise<void> {
        if (!this.card || !this.path) return;

        const entries = [
            ...(this.path === '/' ? [] : [...this.vfsService.splitPath(this.path).reverse().slice(1), this.card.name]),
            'Memory cards',
        ];

        const popover = await this.popoverController.create({
            component: ContextMenuBreadcrumbComponent,
            event: e,
            componentProps: {
                entries,
                onSelect: (index: number) => this.onNavigateBreadcrumb(entries.length - 1 - index),
            },
            dismissOnSelect: true,
            arrow: false,
            side: 'bottom',
            alignment: this.config.get('mode') === 'ios' ? 'center' : 'start',
            trigger: this.breadcrumbTriggerId,
        });

        void popover.present();
    }

    async openContextMenu(e: MouseEvent): Promise<void> {
        const popover = await this.popoverController.create({
            component: ContextMenuDirectoryComponent,
            componentProps: {
                emptyDirectory: this.entries?.length === 0,
                onStartSelection: this.onStartSelection.bind(this),
                onAddFiles: this.onAddFiles.bind(this),
                onCreateDirectry: this.onCreateDirectory.bind(this),
                onExtractZipfile: this.onExtractZipfile.bind(this),
                onPaste: this.onPaste.bind(this),
                onHelp: this.onHelp.bind(this),
            },
            event: e,
            dismissOnSelect: true,
            arrow: false,
        });

        void popover.present();
    }

    private bytesInFilesMemoized = memoize(
        (entries: Array<FileEntry> | undefined): number =>
            entries?.reduce((acc, entry) => acc + (entry.isDirectory ? 0 : entry.size), 0) ?? 0
    );

    private fileCountMemoized = memoize(
        (entries: Array<FileEntry> | undefined): number => entries?.filter((entry) => !entry.isDirectory)?.length ?? 0
    );

    @Input()
    card: StorageCard | undefined;

    @Input()
    path: string | undefined;

    @Input()
    onNavigate: (path: string) => void = () => undefined;

    @Input()
    onNavigateBreadcrumb: (index: number) => void = () => undefined;

    lastEntryTouched: string | undefined = undefined;

    readonly breadcrumbTriggerId: string;

    mode: 'browse' | 'select' = 'browse';

    selection = new Set<string>();

    private checkEntries: () => void;
}
