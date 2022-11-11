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
    onCheck = () => undefined;

    @Input()
    onEject = () => undefined;

    @Input()
    onDelete = () => undefined;

    constructor() {}

    @Input()
    cannotCheck: boolean | undefined;

    @Input()
    isAttached: boolean | undefined;
}
