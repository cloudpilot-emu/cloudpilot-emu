import { Component, Input } from '@angular/core';
import { Session } from '@pwa/model/Session';

@Component({
    selector: 'app-emulation-breadcrumb-menu',
    templateUrl: './breadcrumb-menu.component.html',
    styleUrl: './breadcrumb-menu.component.scss',
    standalone: false,
})
export class BreadcrumbMenuComponent {
    @Input()
    sessions: Array<Session> | undefined;

    @Input()
    onSelect: (session: Session) => void = () => undefined;
}
