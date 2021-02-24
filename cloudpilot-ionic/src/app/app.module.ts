import { IonicModule, IonicRouteStrategy } from '@ionic/angular';

import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { BrowserModule } from '@angular/platform-browser';
import { EmulationService } from './service/emulation.service';
import { NgModule } from '@angular/core';
import { PageLockService } from './service/page-lock.service';
import { RouteReuseStrategy } from '@angular/router';
import { StorageService } from './service/storage.service';

@NgModule({
    declarations: [AppComponent],
    entryComponents: [],
    imports: [BrowserModule, IonicModule.forRoot(), AppRoutingModule],
    providers: [{ provide: RouteReuseStrategy, useClass: IonicRouteStrategy }],
    bootstrap: [AppComponent],
})
export class AppModule {
    constructor(private pageLockService: PageLockService, private storageService: StorageService) {}
}
