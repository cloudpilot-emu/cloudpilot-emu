import { Component } from '@angular/core';
import { REVISION } from './../../../revision.gen';

@Component({
    selector: 'app-settings',
    templateUrl: './settings.page.html',
    styleUrls: ['./settings.page.scss'],
})
export class SettingsPage {
    constructor() {}

    get version(): string {
        return REVISION;
    }
}
