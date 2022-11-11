import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SharedComponentModule } from '@pwa/component/shared-component.module';
import { StorageCardComponent } from './storage-card/storage-card.component';
import { StoragePage } from './storage.page';

@NgModule({
    imports: [CommonModule, IonicModule, FormsModule, ReactiveFormsModule, SharedComponentModule],
    declarations: [StoragePage, StorageCardComponent, ContextMenuComponent],
})
export class StoragePageModule {}
