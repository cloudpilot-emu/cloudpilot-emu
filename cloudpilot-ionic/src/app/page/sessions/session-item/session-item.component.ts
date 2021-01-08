import { Component, Input, OnInit } from '@angular/core';

import { Session } from './../../../model/Session';

@Component({
    selector: 'app-session-item',
    templateUrl: './session-item.component.html',
    styleUrls: ['./session-item.component.scss'],
})
export class SessionItemComponent {
    constructor() {}

    @Input()
    session: Session;
}
