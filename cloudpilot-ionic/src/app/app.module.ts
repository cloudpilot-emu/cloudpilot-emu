import { IonicModule, IonicRouteStrategy } from '@ionic/angular';

import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { BrowserModule } from '@angular/platform-browser';
import { EmulationService } from './service/emulation.service';
import { NgModule } from '@angular/core';
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
        ServiceWorkerModule.register('ngsw-worker.js', { enabled: environment.production }),
    ],
    providers: [{ provide: RouteReuseStrategy, useClass: IonicRouteStrategy }],
    bootstrap: [AppComponent],
})
export class AppModule {
    constructor(
        private pageLockService: PageLockService,
        private storageService: StorageService,
        private emulationService: EmulationService
    ) {}
}
