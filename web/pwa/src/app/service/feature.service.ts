import { Injectable } from '@angular/core';
import { InstallationMode } from '@pwa/model/InstallationMode';
import { PwaService } from './pwa.service';
import { isIndicatorFixApplicable } from '@pwa/helper/homeIndicatorFix';
import { ClipboardService } from './clipboard.service';
import { isIOSNative } from '@common/helper/browser';

@Injectable({ providedIn: 'root' })
export class FeatureService {
    constructor(
        private pwaService: PwaService,
        private clipboardService: ClipboardService,
    ) {
        this.injectStyles();
    }

    get runHidden(): boolean {
        return this.pwaService.getInstallationMode() !== InstallationMode.pwa;
    }

    private featureStyle(feature: string, value: boolean) {
        return (
            (value ? '' : `.feature-${feature} { display: none }\n`) +
            (!value ? '' : `.no-feature-${feature} { display: none }`)
        );
    }

    private injectStyles(): void {
        const styleTag = document.createElement('style');
        styleTag.textContent = `
            ${this.featureStyle('run-hidden', this.runHidden)}
            ${this.featureStyle('indicator-fix', isIndicatorFixApplicable())}
            ${this.featureStyle('clipboard-integration', this.clipboardService.isSupported())}
            ${this.featureStyle('native-app', isIOSNative)}
        `;

        document.head.appendChild(styleTag);
    }
}
