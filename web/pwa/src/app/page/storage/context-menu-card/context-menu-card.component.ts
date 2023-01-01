import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-storage-context-menu-card',
    templateUrl: './context-menu-card.component.html',
    styleUrls: ['./context-menu-card.component.scss'],
})
export class ContextMenuCardComponent {
    constructor() {}

    @Input()
    onEdit: () => void = () => undefined;

    @Input()
    onSave: () => void = () => undefined;

    @Input()
    onCheck: () => void = () => undefined;

    @Input()
    onDelete: () => void = () => undefined;
}
