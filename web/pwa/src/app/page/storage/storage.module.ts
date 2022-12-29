import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuCardComponent } from './context-menu-card/context-menu-card.component';
import { EditCardDialogComponent } from './edit-card-dialog/edit-card-dialog.component';
import { FileEntryComponent } from './file-entry/file-entry.component';
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
        ContextMenuCardComponent,
        NewCardDialogComponent,
        EditCardDialogComponent,
        SubpageCardsComponent,
        SubpageDirectoryComponent,
        FileEntryComponent,
    ],
})
export class StoragePageModule {}
