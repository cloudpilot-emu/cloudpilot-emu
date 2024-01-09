import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-sessions-action-menu',
    templateUrl: './action-menu.component.html',
    styleUrl: './action-menu.component.scss',
})
export class ActionMenuComponent {
    @Input()
    sessionCount = 0;

    @Input()
    onExport: (() => void) | undefined;

    @Input()
    onDelete: (() => void) | undefined;

    @Input()
    onHelp: (() => void) | undefined;
}
