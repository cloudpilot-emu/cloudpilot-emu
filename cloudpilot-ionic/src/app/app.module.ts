import { APP_INITIALIZER, NgModule } from '@angular/core';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { IonicModule, IonicRouteStrategy } from '@ionic/angular';

import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { AudioService } from './service/audio.service';
import { BrowserModule } from '@angular/platform-browser';
import { EmulationService } from './service/emulation.service';
import { KvsService } from './service/kvs.service';
import { MarkdownModule } from 'ngx-markdown';
import { PageLockService } from './service/page-lock.service';
import { RouteReuseStrategy } from '@angular/router';
import { ServiceWorkerModule } from '@angular/service-worker';
import { StorageService } from './service/storage.service';
import { environment } from '../environments/environment';

@NgModule({
    declarations: [AppComponent],
    entryComponents: [],
    imports: [
        BrowserModule,
        IonicModule.forRoot(),
        AppRoutingModule,
        ServiceWorkerModule.register('ngsw-worker.js', {
            enabled: environment.production,
            registrationStrategy: 'registerImmediately',
        }),
        MarkdownModule.forRoot({ loader: HttpClient }),
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
