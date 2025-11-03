import { HttpClient, provideHttpClient, withInterceptorsFromDi } from '@angular/common/http';
import { NgModule, inject, provideAppInitializer } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { RouteReuseStrategy } from '@angular/router';
import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { IonicModule, IonicRouteStrategy } from '@ionic/angular';
import wasmModule from '@native/cloudpilot_web.wasm';
import { createDirectives } from 'marked-directive';
import { MARKED_EXTENSIONS, MARKED_OPTIONS, MarkdownModule, MarkedOptions, MarkedRenderer } from 'ngx-markdown';

import { AppRoutingModule } from '@pwa/app-routing.module';
import { AppComponent } from '@pwa/app.component';
import { DummyComponent } from '@pwa/component/dummy/dummy.component';
import { Lock } from '@pwa/helper/lock';
import { AudioService } from '@pwa/service/audio.service';
import { AutoEnableAudioService } from '@pwa/service/auto-enable-audio.service';
import { EmulationService } from '@pwa/service/emulation.service';
import { KvsService } from '@pwa/service/kvs.service';
import { ServiceWorkerService } from '@pwa/service/service-worker.service';
import { SessionService } from '@pwa/service/session.service';
import { StorageService } from '@pwa/service/storage.service';
import { CLOUDPILOT_INSTANCE_TOKEN, EMULATOR_LOCK_TOKEN } from '@pwa/service/token';
import { UpdateService } from '@pwa/service/update.service';

declare global {
    interface Window {
        __cp_enableLogging?: (logging: boolean) => void;
    }
}

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
        {
            provide: CLOUDPILOT_INSTANCE_TOKEN,
            useFactory: () => {
                const instance = Cloudpilot.create(wasmModule);
                window.__cp_enableLogging = (logging) => instance.then((cp) => cp.enableLogging(logging));

                return instance;
            },
        },
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
