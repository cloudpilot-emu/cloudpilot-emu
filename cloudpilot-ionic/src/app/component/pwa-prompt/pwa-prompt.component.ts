import { Component, OnInit } from '@angular/core';
import { isAndroid, isIOS } from './../../helper/browser';

import { PwaService } from './../../service/pwa.service';

@Component({
    selector: 'app-pwa-prompt',
    templateUrl: './pwa-prompt.component.html',
    styleUrls: ['./pwa-prompt.component.scss'],
})
export class PwaPromptComponent {
    constructor(private pwaService: PwaService) {}

    onClick(): void {
        this.pwaService.install();
    }

    get iconName(): string {
        if (isIOS) return 'logo-apple-appstore';
        if (isAndroid) return 'logo-google-playstore';

        return 'apps-outline';
    }

    get show(): boolean {
        return this.pwaService.promptForInstall();
    }
}
