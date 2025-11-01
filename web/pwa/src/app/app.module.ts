import { HttpClient, provideHttpClient, withInterceptorsFromDi } from '@angular/common/http';
import { NgModule, inject, provideAppInitializer } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { RouteReuseStrategy } from '@angular/router';
import { IonicModule, IonicRouteStrategy } from '@ionic/angular';
import { createDirectives } from 'marked-directive';
import { MARKED_EXTENSIONS, MARKED_OPTIONS, MarkdownModule, MarkedOptions, MarkedRenderer } from 'ngx-markdown';

import { DummyComponent } from '@pwa/component/dummy/dummy.component';
import { AudioService } from '@pwa/service/audio.service';
import { AutoEnableAudioService } from '@pwa/service/auto-enable-audio.service';
import { EmulationService } from '@pwa/service/emulation.service';
import { KvsService } from '@pwa/service/kvs.service';
import { SessionService } from '@pwa/service/session.service';
import { StorageService } from '@pwa/service/storage.service';
import { UpdateService } from '@pwa/service/update.service';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { EMULATOR_LOCK_TOKEN, Lock } from './helper/lock';
import { ServiceWorkerService } from './service/service-worker.service';

const markedOptionsFactory = (): MarkedOptions => {
    const renderer = new MarkedRenderer();

    renderer.link = function ({ href, tokens }) {
        return `<a href="${href}" target="_blank" rel="external">${this.parser.parseInline(tokens) || href}</a>`;
    };

    return {
        renderer,
        gfm: true,
        breaks: false,
        pedantic: false,
    };
};

@NgModule({
    declarations: [AppComponent, DummyComponent],
    bootstrap: [AppComponent],
    imports: [
        BrowserModule,
        IonicModule.forRoot({ innerHTMLTemplatesEnabled: true }),
        AppRoutingModule,
        MarkdownModule.forRoot({
            loader: HttpClient,
            markedOptions: { provide: MARKED_OPTIONS, useFactory: markedOptionsFactory },
            markedExtensions: [
                {
                    provide: MARKED_EXTENSIONS,
                    useFactory: createDirectives,
                    multi: true,
                },
            ],
        }),
    ],
    providers: [
        { provide: RouteReuseStrategy, useClass: IonicRouteStrategy },
        { provide: EMULATOR_LOCK_TOKEN, useClass: Lock },
        provideAppInitializer(() => {
            const initializerFn = ((kvsService: KvsService) => kvsService.initialize.bind(kvsService))(
                inject(KvsService),
            );
            return initializerFn();
        }),
        provideHttpClient(withInterceptorsFromDi()),
    ],
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
