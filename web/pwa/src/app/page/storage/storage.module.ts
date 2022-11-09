import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SharedComponentModule } from '@pwa/component/shared-component.module';
import { StoragePage } from './storage.page';

@NgModule({
    imports: [CommonModule, IonicModule, FormsModule, ReactiveFormsModule, SharedComponentModule],
    declarations: [StoragePage],
})
export class StoragePageModule {}
