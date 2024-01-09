import { Component, Input } from '@angular/core';

@Component({
    selector: 'app-storage-action-menu-cards',
    templateUrl: './action-menu-cards.component.html',
    styleUrl: './action-menu-cards.component.scss',
})
export class ActionMenuCardsComponent {
    @Input()
    onExport: (() => void) | undefined;

    @Input()
    onDelete: (() => void) | undefined;

    @Input()
    onHelp: (() => void) | undefined;
}
