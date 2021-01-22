import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';

import { DeviceId } from 'src/app/model/DeviceId';
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

    @Output()
    delete = new EventEmitter<Session>();

    @Output()
    edit = new EventEmitter<Session>();

    @Output()
    save = new EventEmitter<Session>();

    get device(): string {
        switch (this.session.device) {
            case DeviceId.m515:
                return 'Palm m515';

            case DeviceId.palmV:
                return 'Palm V';

            default:
                throw new Error('bad device ID');
        }
    }
}
