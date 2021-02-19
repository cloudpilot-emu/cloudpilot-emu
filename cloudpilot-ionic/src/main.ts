import { AppModule } from './app/app.module';
import { enableProdMode } from '@angular/core';
import { environment } from './environments/environment';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

if (environment.production) {
    enableProdMode();
}

platformBrowserDynamic()
    .bootstrapModule(AppModule)
    .catch((err) => console.log(err));

// work around status bar overlapping the viewport after rotation on iOS

window.addEventListener('orientationchange', () =>
    setTimeout(() => {
        document.body.style.height = window.innerHeight + 'px';
    }, 100)
);

window.addEventListener('scroll', () => window.scrollTo(0, 0));
