import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';

import { AboutPageModule } from '@pwa/page/about/about.module';
import { EmulationPageModule } from '@pwa/page/emulation/emulation.module';
import { SessionsPageModule } from '@pwa/page/sessions/sessions.module';
import { SettingsPageModule } from '@pwa/page/settings/settings.module';
import { StoragePageModule } from '@pwa/page/storage/storage.module';

import { TabsPageRoutingModule } from './tabs-routing.module';
import { TabsPage } from './tabs.page';

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
