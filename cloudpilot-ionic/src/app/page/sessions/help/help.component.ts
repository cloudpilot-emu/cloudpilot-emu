import { Component, OnInit } from '@angular/core';

import { ModalController } from '@ionic/angular';

@Component({
    selector: 'app-sessions-help',
    templateUrl: './help.component.html',
    styleUrls: ['./help.component.scss'],
})
export class HelpComponent {
    constructor(public modalController: ModalController) {}
}
