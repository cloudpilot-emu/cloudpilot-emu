import { Component, EventEmitter, HostBinding, HostListener, Input, Output } from '@angular/core';

let TRIGGER_INDEX = 0;

@Component({
    selector: 'app-breadcrumb',
    templateUrl: './breadcrumb.component.html',
    styleUrl: './breadcrumb.component.scss',
})
export class BreadcrumbComponent {
    @HostListener('click', ['$event'])
    private handleClick(e: MouseEvent): void {
        if (this.disabled) return;

        e.preventDefault();
        this.trigger.emit(this.triggerId);
    }

    @Input()
    @HostBinding('class.disabled')
    disabled = false;

    @Output()
    trigger = new EventEmitter<string>();

    @HostBinding('attr.id')
    readonly triggerId = `breadcrumb-trigger-${TRIGGER_INDEX++}`;
}
