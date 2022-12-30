import { Component, Input } from '@angular/core';

import { Config } from '@ionic/angular';

@Component({
    selector: 'app-session-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
})
export class ContextMenuComponent {
    constructor(private config: Config) {}

    get color(): string | undefined {
        return this.config.get('mode') === 'ios' ? 'light' : undefined;
    }

    @Input()
    public onEdit: () => void = () => undefined;

    @Input()
    public onSave: () => void = () => undefined;

    @Input()
    public onDelete: () => void = () => undefined;

    @Input()
    public onReset: () => void = () => undefined;
}
