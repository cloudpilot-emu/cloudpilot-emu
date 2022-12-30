import { Component, Input } from '@angular/core';

import { Config } from '@ionic/angular';

@Component({
    selector: 'app-storage-context-menu-card',
    templateUrl: './context-menu-card.component.html',
    styleUrls: ['./context-menu-card.component.scss'],
})
export class ContextMenuCardComponent {
    constructor(private config: Config) {}

    get color(): string | undefined {
        return this.config.get('mode') === 'ios' ? 'light' : undefined;
    }

    @Input()
    onEdit = () => undefined;

    @Input()
    onSave = () => undefined;

    @Input()
    onCheck = () => undefined;

    @Input()
    onDelete = () => undefined;
}
