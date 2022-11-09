import { AboutPageModule } from '@pwa/page/about/about.module';
import { CommonModule } from '@angular/common';
import { EmulationPageModule } from '@pwa/page/emulation/emulation.module';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SessionsPageModule } from '@pwa/page/sessions/sessions.module';
import { SettingsPageModule } from '@pwa/page/settings/settings.module';
import { StoragePageModule } from './../page/storage/storage.module';
import { TabsPage } from './tabs.page';
import { TabsPageRoutingModule } from './tabs-routing.module';

@NgModule({
    imports: [
        IonicModule,
        CommonModule,
        FormsModule,
        TabsPageRoutingModule,
        EmulationPageModule,
        SessionsPageModule,
        SettingsPageModule,
        StoragePageModule,
        AboutPageModule,
    ],
    declarations: [TabsPage],
})
export class TabsPageModule {}
