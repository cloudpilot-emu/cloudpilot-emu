import './app/helper/pagelock';

import { AppModule } from './app/app.module';
import { REVISION } from './revision';
import { enableProdMode } from '@angular/core';
import { environment } from './environments/environment';
import { hasStoredSession } from './app/helper/storedSession';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

console.log(REVISION);

if (hasStoredSession()) {
    const url = new URL(location.toString());

    location.replace(`${location.origin}${location.pathname}#/tab/emulation`);
} else {
    location.replace(`${location.origin}${location.pathname}#/tab/sessions`);
}

if (environment.production) {
    enableProdMode();
}

// work around status bar overlapping the viewport after rotation on iOS

window.addEventListener('orientationchange', () =>
    setTimeout(() => {
        document.body.style.height = window.innerHeight + 'px';
    }, 100)
);

window.addEventListener('scroll', () => window.scrollTo(0, 0));

platformBrowserDynamic()
    .bootstrapModule(AppModule)
    .catch((err) => console.log(err));
