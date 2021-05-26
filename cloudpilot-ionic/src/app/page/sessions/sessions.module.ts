import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SessionItemComponent } from './session-item/session-item.component';
import { SessionSettingsComponent } from './settings/settings.component';
import { SessionsPage } from './sessions.page';
import { SharedComponentModule } from './../../component/shared-component.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, ReactiveFormsModule, SharedComponentModule],
    declarations: [SessionsPage, SessionItemComponent, ContextMenuComponent, SessionSettingsComponent],
})
export class SessionsPageModule {}
