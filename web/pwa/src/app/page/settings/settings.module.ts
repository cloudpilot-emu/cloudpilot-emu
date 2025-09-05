import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';

import { SharedComponentModule } from '@pwa/component/shared-component.module';

import { SettingsPage } from './settings.page';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SharedComponentModule, ReactiveFormsModule],
    declarations: [SettingsPage],
})
export class SettingsPageModule {}
