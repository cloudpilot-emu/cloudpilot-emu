import { Component, Input } from '@angular/core';

import { ModalController } from '@ionic/angular';

@Component({
    selector: 'app-help',
    templateUrl: './help.component.html',
    styleUrls: ['./help.component.scss'],
    standalone: false
})
export class HelpComponent {
    @Input()
    public url!: string;

    @Input()
    public title = 'Help';

    public loading = true;

    constructor(public modalController: ModalController) {}
}
