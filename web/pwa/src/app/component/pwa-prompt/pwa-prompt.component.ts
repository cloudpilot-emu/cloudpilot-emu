import { isAndroid, isIOS } from '@common/helper/browser';

import { Component } from '@angular/core';
import { PwaService } from '@pwa/service/pwa.service';

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
