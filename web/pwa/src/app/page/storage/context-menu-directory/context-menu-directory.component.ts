import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-context-menu-directory',
    templateUrl: './context-menu-directory.component.html',
    styleUrls: ['./context-menu-directory.component.scss'],
})
export class ContextMenuDirectoryComponent {
    constructor() {}

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
}
