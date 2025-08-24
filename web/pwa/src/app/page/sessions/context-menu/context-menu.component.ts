import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-session-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
    standalone: false
})
export class ContextMenuComponent {
    constructor() {}

    @Input()
    public onEdit: () => void = () => undefined;

    @Input()
    public onSave: () => void = () => undefined;

    @Input()
    public onDelete: () => void = () => undefined;

    @Input()
    public onReset: () => void = () => undefined;
}
