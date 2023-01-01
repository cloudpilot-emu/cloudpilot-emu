import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-context-menu-file-entry',
    templateUrl: './context-menu-file-entry.component.html',
    styleUrls: ['./context-menu-file-entry.component.scss'],
})
export class ContextMenuFileEntryComponent {
    constructor() {}

    @Input()
    onEdit: () => void = () => undefined;

    @Input()
    onSave: () => void = () => undefined;

    @Input()
    onCopy: () => void = () => undefined;

    @Input()
    onCut: () => void = () => undefined;

    @Input()
    onDelete: () => void = () => undefined;
}
