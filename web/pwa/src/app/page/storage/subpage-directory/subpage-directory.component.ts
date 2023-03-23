import { ActionSheetController, Config, ModalController, PopoverController } from '@ionic/angular';
import { ChangeDetectionStrategy, ChangeDetectorRef, Component, DoCheck, Input, OnInit } from '@angular/core';

import { AlertService } from '@pwa/service/alert.service';
import { ContextMenuBreadcrumbComponent } from './../context-menu-breadcrumb/context-menu-breadcrumb.component';
import { ContextMenuDirectoryComponent } from './../context-menu-directory/context-menu-directory.component';
import { EditFileDialogComponent } from './../edit-file-dialog/edit-file-dialog.component';
import { FileDescriptor } from './../../../service/file.service';
import { FileEntry } from '@common/bridge/Vfs';
import { FileService } from '@pwa/service/file.service';
import { NewDirectoryDialogComponent } from './../new-directory-dialog/new-directory-dialog.component';
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
export class SubpageDirectoryComponent implements DoCheck, OnInit {
    constructor(
        private vfsService: VfsService,
        private popoverController: PopoverController,
        private config: Config,
        private alertService: AlertService,
        private actionSheetController: ActionSheetController,
        private modalController: ModalController,
        private fileService: FileService,
        private cd: ChangeDetectorRef
    ) {
        this.breadcrumbTriggerId = `breadcrumb-trigger-${BREADCRUMB_TRIGGER_INDEX++}`;

        this.checkEntries = changeDetector(cd, undefined, () =>
            this.path !== undefined ? vfsService.readdir(this.path) : undefined
        );
    }

    ngOnInit(): void {
        if (this.selfReference) this.selfReference.ref = this;
    }

    ngDoCheck(): void {
        this.checkEntries();
    }

    handleDragDropEvent(e: DragEvent): void | Promise<void> {
        this.fileService.openFromDrop(e, (files) => this.vfsService.addFiles(files, this.path || ''));
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

    @debounce()
    async onSaveEntry(entry: FileEntry): Promise<void> {
        if (!this.path) return;

        if (entry.isDirectory) {
            await this.vfsService.archiveFiles([entry], this.path);
        } else {
            await this.vfsService.saveFile(entry);
        }
    }

    onCopyEntry(entry: FileEntry): void {
        if (!this.path) return;

        this.vfsService.copyToClipboard(this.path, [entry.name], 'copy');
    }

    onCutEntry(entry: FileEntry): void {
        if (!this.path) return;

        this.vfsService.copyToClipboard(this.path, [entry.name], 'cut');
    }

    @debounce()
    async onDeleteEntry(entry: FileEntry): Promise<void> {
        if (entry.isDirectory) {
            await this.vfsService.deleteRecursive([entry]);
        } else {
            await this.vfsService.unlinkEntry(entry);
        }
    }

    onAddFiles(): void {
        this.fileService.openFiles((files) => this.vfsService.addFiles(files, this.path || '/'));
    }

    @debounce()
    async onCreateDirectory(): Promise<void> {
        const modal = await this.modalController.create({
            component: NewDirectoryDialogComponent,
            backdropDismiss: false,
            componentProps: {
                parentPath: this.path,
                onCreate: (name: string) => this.vfsService.mkdir(`${this.path}/${name}`),
            },
        });

        await modal.present();
    }

    onExtractZipfile(): void {
        this.fileService.openFile(this.extractZipArchive.bind(this));
    }

    @debounce()
    async onPaste(): Promise<void> {
        if (this.path) await this.vfsService.paste(this.path);
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
        if (!this.path) return;

        this.vfsService.copyToClipboard(this.path, Array.from(this.selection), 'copy');
    }

    onCutSelection(): void {
        if (!this.path) return;

        this.vfsService.copyToClipboard(this.path, Array.from(this.selection), 'cut');
    }

    @debounce()
    async onSaveZip(): Promise<void> {
        const selection = this.materializeSelection();
        if (!selection || !this.path) return;

        await this.vfsService.archiveFiles(selection, this.path);
    }

    @debounce()
    async onDeleteSelection(): Promise<void> {
        const selection = this.materializeSelection();
        if (!selection || !this.path) return;

        await this.vfsService.deleteRecursive(selection);
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

    private materializeSelection(): Array<FileEntry> | undefined {
        if (!this.path) return;

        const entriesByName = new Map<string, FileEntry>(
            this.vfsService.readdir(this.path).map((entry) => [entry.name, entry])
        );

        return Array.from(this.selection)
            .map((name) => entriesByName.get(name))
            .filter((entry) => !!entry) as Array<FileEntry>;
    }

    private async extractZipArchive(file: FileDescriptor): Promise<void> {
        if (!/\.zip$/.test(file.name)) {
            await this.alertService.errorMessage(
                'File does not have the correct extension. Only .zip files are supported'
            );

            return;
        }

        void this.vfsService.unpackArchive(await file.getContent(), this.path || '/');
    }

    @Input()
    card: StorageCard | undefined;

    @Input()
    path: string | undefined;

    @Input()
    selfReference: { ref: SubpageDirectoryComponent } | undefined;

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
