import { Component, Input } from '@angular/core';

import { Config } from '@ionic/angular';

@Component({
    selector: 'app-context-menu-breadcrumb',
    templateUrl: './context-menu-breadcrumb.component.html',
    styleUrls: ['./context-menu-breadcrumb.component.scss'],
})
export class ContextMenuBreadcrumbComponent {
    constructor(private config: Config) {}

    get color(): string | undefined {
        return this.config.get('mode') === 'ios' ? 'light' : undefined;
    }

    @Input()
    entries: Array<string> | undefined;

    onSelect: (index: number) => void = () => undefined;
}
