import { AboutPageModule } from './../page/about/about.module';
import { CommonModule } from '@angular/common';
import { EmulationPageModule } from './../page/emulation/emulation.module';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SessionsPageModule } from './../page/sessions/sessions.module';
import { SettingsPageModule } from './../page/settings/settings.module';
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
        AboutPageModule,
    ],
    declarations: [TabsPage],
})
export class TabsPageModule {}
