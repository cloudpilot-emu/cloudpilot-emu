import { Component, OnInit } from '@angular/core';

import { SessionService } from 'src/app/service/session.service';

@Component({
    selector: 'app-sessions',
    templateUrl: './sessions.page.html',
    styleUrls: ['./sessions.page.scss'],
})
export class SessionsPage implements OnInit {
    constructor(public sessionService: SessionService) {}

    ngOnInit() {}
}
