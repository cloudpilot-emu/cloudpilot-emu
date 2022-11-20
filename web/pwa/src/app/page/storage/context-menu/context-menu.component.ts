import { Component, EventEmitter, Input, Output } from '@angular/core';

import { StorageCard } from '@pwa/model/StorageCard';

@Component({
    selector: 'app-storage-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
})
export class ContextMenuComponent {
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
