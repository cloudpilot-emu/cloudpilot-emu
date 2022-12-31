import { Component, Input, OnInit } from '@angular/core';
import { Config, PopoverController } from '@ionic/angular';

import { ContextMenuBreadcrumbComponent } from './../context-menu-breadcrumb/context-menu-breadcrumb.component';
import { FileEntry } from '@common/bridge/Vfs';
import { StorageCard } from '@pwa/model/StorageCard';
import { VfsService } from '@pwa/service/vfs.service';

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
})
export class SubpageDirectoryComponent implements OnInit {
    constructor(private vfsService: VfsService, private popoverController: PopoverController, private config: Config) {
        this.breadcrumbTriggerId = `breadcrumb-trigger-${BREADCRUMB_TRIGGER_INDEX++}`;
    }

    showHelp(): void {}

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

    get showBreadcrumb(): boolean {
        return !!this.path && this.path !== '/';
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

    async openBreadcrumbMenu(e: MouseEvent): Promise<void> {
        if (!this.card || !this.path || this.path === '/') return;

        const entries = [
            ...[...this.vfsService.splitPath(this.path).reverse().slice(1), this.card.name],
            'Storage cards',
        ];

        const popover = await this.popoverController.create({
            component: ContextMenuBreadcrumbComponent,
            event: e,
            backdropDismiss: true,
            showBackdrop: false,
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

    private updateEntries(): void {
        if (this.path === undefined) {
            this.entries = [];
            return;
        }

        void this.vfsService
            .readdir(this.path)
            .then((entries) => (this.entries = entries ? [...entries].sort(entrySortFunction) : []));
    }

    @Input()
    card: StorageCard | undefined;

    @Input()
    path: string | undefined;

    @Input()
    onNavigate: (path: string) => void = () => undefined;

    @Input()
    onNavigateBreadcrumb: (index: number) => void = () => undefined;

    entries: Array<FileEntry> | undefined;

    lastEntryTouched: string | undefined = undefined;

    readonly breadcrumbTriggerId: string;
}
