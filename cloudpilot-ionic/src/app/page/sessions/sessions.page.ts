import { Component, OnInit } from '@angular/core';

import { Device } from 'src/app/model/Device';
import { Session } from 'src/app/model/Session';
import { SessionService } from 'src/app/service/session.service';

@Component({
    selector: 'app-sessions',
    templateUrl: './sessions.page.html',
    styleUrls: ['./sessions.page.scss'],
})
export class SessionsPage implements OnInit {
    constructor(public sessionService: SessionService) {}

    ngOnInit(): void {}
}
