import { Component, Input, OnInit } from '@angular/core';

@Component({
    selector: 'app-session-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
})
export class ContextMenuComponent implements OnInit {
    @Input()
    public onEdit: () => void = () => undefined;

    @Input()
    public onSave: () => void = () => undefined;

    @Input()
    public onDelete: () => void = () => undefined;

    @Input()
    public onReset: () => void = () => undefined;

    constructor() {}

    ngOnInit() {}
}
