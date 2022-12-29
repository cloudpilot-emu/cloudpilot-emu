import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-storage-context-menu-card',
    templateUrl: './context-menu-card.component.html',
    styleUrls: ['./context-menu-card.component.scss'],
})
export class ContextMenuCardComponent {
    @Input()
    onEdit = () => undefined;

    @Input()
    onSave = () => undefined;

    @Input()
    onCheck = () => undefined;

    @Input()
    onDelete = () => undefined;

    constructor() {}
}
