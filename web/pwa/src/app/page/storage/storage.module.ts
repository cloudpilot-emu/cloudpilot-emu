import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EditCardDialogComponent } from './edit-card-dialog/edit-card-dialog.component';
import { IonicModule } from '@ionic/angular';
import { NewCardDialogComponent } from './new-card-dialog/new-card-dialog.component';
import { NgModule } from '@angular/core';
import { SharedComponentModule } from '@pwa/component/shared-component.module';
import { StorageCardComponent } from './storage-card/storage-card.component';
import { StoragePage } from './storage.page';
import { SubpageCardsComponent } from './subpage-cards/subpage-cards.component';
import { SubpageDirectoryComponent } from './subpage-directory/subpage-directory.component';

@NgModule({
    imports: [CommonModule, IonicModule, FormsModule, ReactiveFormsModule, SharedComponentModule],
    declarations: [
        StoragePage,
        StorageCardComponent,
        ContextMenuComponent,
        NewCardDialogComponent,
        EditCardDialogComponent,
        SubpageCardsComponent,
        SubpageDirectoryComponent,
    ],
})
export class StoragePageModule {}
