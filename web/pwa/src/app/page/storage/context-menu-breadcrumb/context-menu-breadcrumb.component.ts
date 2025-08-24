import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-context-menu-breadcrumb',
    templateUrl: './context-menu-breadcrumb.component.html',
    styleUrls: ['./context-menu-breadcrumb.component.scss'],
    standalone: false
})
export class ContextMenuBreadcrumbComponent {
    constructor() {}

    @Input()
    entries: Array<string> | undefined;

    onSelect: (index: number) => void = () => undefined;
}
