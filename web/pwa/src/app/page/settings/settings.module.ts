import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SettingsPage } from './settings.page';
import { SharedComponentModule } from '@pwa/component/shared-component.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SharedComponentModule, FormsModule, ReactiveFormsModule],
    declarations: [SettingsPage],
})
export class SettingsPageModule {}
