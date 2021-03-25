import { Component } from '@angular/core';
import { VERSION } from '../../../app/helper/version';

@Component({
    selector: 'app-about',
    templateUrl: './about.page.html',
    styleUrls: ['./about.page.scss'],
})
export class AboutPage {
    constructor() {}

    get version(): string {
        return VERSION;
    }
}
