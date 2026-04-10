import { Injectable } from '@angular/core';
import { isAndroid, isIOS, isIOSNative, isMacOSSafari } from '@common/helper/browser';

import { dynamicFontsSupport } from '@pwa/helper/dynamicFonts';
import { isIndicatorFixApplicable } from '@pwa/helper/homeIndicatorFix';

import { ClipboardService } from './clipboard.service';
import { PlatformService } from './platform.service';

@Injectable({ providedIn: 'root' })
export class FeatureService {
    constructor(
        private platformService: PlatformService,
        private clipboardService: ClipboardService,
    ) {
        this.injectStyles();
    }

    get runHidden(): boolean {
        return !(isIOS || isAndroid);
    }

    get nativeNetworkIntegration(): boolean {
        return this.platformService.supportsNativeNetworkIntegration();
    }

    get nativeClipboard(): boolean {
        return this.platformService.supportsNativeClipboard();
    }

    get channelManagement(): boolean {
        return this.platformService.supportsChannelManagement();
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
            ${this.featureStyle('ios-not-native', isIOS && !isIOSNative)}
            ${this.featureStyle('ios-or-safari', (isIOS && !isIOSNative) || isMacOSSafari)}
            ${this.featureStyle('ios', isIOS)}
            ${this.featureStyle('native-network', this.nativeNetworkIntegration)}
            ${this.featureStyle('dynamic-font-size', dynamicFontsSupport())}
            ${this.featureStyle('channel-management', this.channelManagement)}
        `;

        document.head.appendChild(styleTag);
    }
}
