import { APP_INITIALIZER, NgModule } from '@angular/core';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { IonicModule, IonicRouteStrategy } from '@ionic/angular';
import { MARKED_OPTIONS, MarkdownModule, MarkedOptions, MarkedRenderer } from 'ngx-markdown';

import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { AudioService } from '@pwa/service/audio.service';
import { AutoEnableAudioService } from '@pwa/service/auto-enable-audio.service';
import { BrowserModule } from '@angular/platform-browser';
import { DummyComponent } from '@pwa/component/dummy/dummy.component';
import { EmulationService } from '@pwa/service/emulation.service';
import { KvsService } from '@pwa/service/kvs.service';
import { RouteReuseStrategy } from '@angular/router';
import { SessionService } from '@pwa/service/session.service';
import { StorageService } from '@pwa/service/storage.service';
import { UpdateService } from '@pwa/service/update.service';
import { ionAnimationConfig } from './helper/reducedAnimations';
import { ServiceWorkerService } from './service/service-worker.service';
import { createDirectives } from 'marked-directive';

const markedOptionsFactory = (): MarkedOptions => {
    const renderer = new MarkedRenderer();

    renderer.link = (href, title, text) => `<a href="${href}" target="_blank" rel="external">${text || href}</a>`;

    return {
        renderer,
        gfm: true,
        breaks: false,
        pedantic: false,
    };
};

@NgModule({
    declarations: [AppComponent, DummyComponent],
    imports: [
        BrowserModule,
        IonicModule.forRoot({ ...ionAnimationConfig(), innerHTMLTemplatesEnabled: true }),
        AppRoutingModule,
        MarkdownModule.forRoot({
            loader: HttpClient,
            markedOptions: { provide: MARKED_OPTIONS, useFactory: markedOptionsFactory },
            markedExtensions: [createDirectives()],
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
    /* eslint-disable @typescript-eslint/no-unused-vars */
    constructor(
        storageService: StorageService,
        emulationService: EmulationService,
        audioService: AudioService,
        sessionService: SessionService,
        kvs: KvsService,
        updateService: UpdateService,
        autoEnableAudioService: AutoEnableAudioService,
        serviceWorkerService: ServiceWorkerService,
    ) {}
    /* eslint-enable @typescript-eslint/no-unused-vars */
}
