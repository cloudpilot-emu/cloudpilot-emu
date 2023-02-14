import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuBreadcrumbComponent } from './context-menu-breadcrumb/context-menu-breadcrumb.component';
import { ContextMenuCardComponent } from './context-menu-card/context-menu-card.component';
import { ContextMenuDirectoryComponent } from './context-menu-directory/context-menu-directory.component';
import { ContextMenuFileEntryComponent } from './context-menu-file-entry/context-menu-file-entry.component';
import { EditCardDialogComponent } from './edit-card-dialog/edit-card-dialog.component';
import { EditFileDialogComponent } from './edit-file-dialog/edit-file-dialog.component';
import { FileEntryComponent } from './file-entry/file-entry.component';
import { IonicModule } from '@ionic/angular';
import { NewCardDialogComponent } from './new-card-dialog/new-card-dialog.component';
import { NewDirectoryDialogComponent } from './new-directory-dialog/new-directory-dialog.component';
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
        ContextMenuBreadcrumbComponent,
        ContextMenuFileEntryComponent,
        ContextMenuDirectoryComponent,
        EditFileDialogComponent,
        NewDirectoryDialogComponent,
    ],
})
export class StoragePageModule {}
