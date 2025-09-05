import { ApplicationRef, enableProdMode } from '@angular/core';
import { enableDebugTools } from '@angular/platform-browser';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';
import { isIOS } from '@common/helper/browser';

import { initializeDynamicFOnts as initializeDynamicFonts } from '@pwa/helper/dynamicFonts';
import { applyHomeIndicatorFix } from '@pwa/helper/homeIndicatorFix';
import { hasStoredSession } from '@pwa/helper/storedSession';
import { bootstrapLinkApi, hasInitialImportRequest, hasInitialInstallRequest } from '@pwa/service/link-api.service';

import { AppModule } from './app/app.module';
import { environment } from './environments/environment';

// Work around issue with webpack and deep-equal
// eslint-disable-next-line @typescript-eslint/no-explicit-any
(window as any).global = window;

// Prevernt zoom-by-doubletap on iOS (and any other unwanted default interactions)
document.body.addEventListener('dblclick', (e) => e.preventDefault());

initializeDynamicFonts();

bootstrapLinkApi();

if ((hasStoredSession() && !hasInitialImportRequest()) || hasInitialInstallRequest()) {
    location.replace(`${location.origin}${location.pathname}#/tab/emulation`);
} else {
    location.replace(`${location.origin}${location.pathname}#/tab/sessions`);
}

if (environment.production) {
    enableProdMode();
}

if (isIOS) {
    if (CSS.supports('height', '100dvh')) {
        document.body.style.height = '100dvh';
    } else {
        window.addEventListener('orientationchange', () =>
            [100, 300, 500].forEach((timeout) =>
                setTimeout(() => {
                    document.body.style.height = window.innerHeight + 'px';
                }, timeout),
            ),
        );
    }

    applyHomeIndicatorFix();
}

window.addEventListener('scroll', () => window.scrollTo(0, 0));
document.body.addEventListener('contextmenu', (e) => e.preventDefault());

platformBrowserDynamic()
    .bootstrapModule(AppModule)
    .then((module) => environment.production || enableDebugTools(module.injector.get(ApplicationRef).components[0]))
    .catch((err) => console.log(err));
