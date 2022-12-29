import { Component, Input } from '@angular/core';

import { StorageCard } from '@pwa/model/StorageCard';

@Component({
    selector: 'app-subpage-directory',
    templateUrl: './subpage-directory.component.html',
    styleUrls: ['./subpage-directory.component.scss'],
})
export class SubpageDirectoryComponent {
    showHelp() {}

    constructor() {}

    @Input()
    card: StorageCard | undefined;
}
