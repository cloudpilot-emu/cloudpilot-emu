import { APP_INITIALIZER, NgModule } from '@angular/core';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { IonicModule, IonicRouteStrategy } from '@ionic/angular';
import { MarkdownModule, MarkedOptions, MarkedRenderer } from 'ngx-markdown';

import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { AudioService } from './service/audio.service';
import { BrowserModule } from '@angular/platform-browser';
import { EmulationService } from './service/emulation.service';
import { KvsService } from './service/kvs.service';
import { PageLockService } from './service/page-lock.service';
import { RouteReuseStrategy } from '@angular/router';
import { ServiceWorkerModule } from '@angular/service-worker';
import { StorageService } from './service/storage.service';
import { environment } from '../environments/environment';

const markedOptionsFactory = (): MarkedOptions => {
    const renderer = new MarkedRenderer();

    renderer.link = (href, title, text) => `<a href="${href}" target="_blank" rel="external">${text || href}</a>`;

    return {
        renderer,
        gfm: true,
        breaks: false,
        pedantic: false,
        smartLists: true,
        smartypants: false,
    };
};

@NgModule({
    declarations: [AppComponent],
    entryComponents: [],
    imports: [
        BrowserModule,
        IonicModule.forRoot(),
        AppRoutingModule,
        ServiceWorkerModule.register('ngsw-worker.js', { enabled: environment.production }),
        MarkdownModule.forRoot({
            loader: HttpClient,
            markedOptions: { provide: MarkedOptions, useFactory: markedOptionsFactory },
        }),
        HttpClientModule,
    ],
    providers: [
        { provide: RouteReuseStrategy, useClass: IonicRouteStrategy },
        {
            provide: APP_INITIALIZER,
            multi: true,
            useFactory: (kvsService: KvsService) => kvsService.initialize.bind(kvsService),
            deps: [KvsService],
        },
    ],
    bootstrap: [AppComponent],
})
export class AppModule {
    constructor(
        pageLockService: PageLockService,
        storageService: StorageService,
        emulationService: EmulationService,
        audioService: AudioService
    ) {}
}
