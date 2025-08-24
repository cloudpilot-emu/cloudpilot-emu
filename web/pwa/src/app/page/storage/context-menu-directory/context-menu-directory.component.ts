import { Component, Input } from '@angular/core';

import { VfsService } from '@pwa/service/vfs.service';

@Component({
    selector: 'app-context-menu-directory',
    templateUrl: './context-menu-directory.component.html',
    styleUrls: ['./context-menu-directory.component.scss'],
    standalone: false
})
export class ContextMenuDirectoryComponent {
    constructor(public vfsService: VfsService) {}

    @Input()
    onStartSelection: () => void = () => undefined;

    @Input()
    onAddFiles: () => void = () => undefined;

    @Input()
    onCreateDirectry: () => void = () => undefined;

    @Input()
    onExtractZipfile: () => void = () => undefined;

    @Input()
    onPaste: () => void = () => undefined;

    @Input()
    onHelp: () => void = () => undefined;

    @Input()
    emptyDirectory = false;
}
