import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SessionItemComponent } from './session-item/session-item.component';
import { SessionSettingsComponent } from './settings/settings/settings.component';
import { SessionsPage } from './sessions.page';
import { SessionsPageRoutingModule } from './sessions-routing.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SessionsPageRoutingModule, ReactiveFormsModule],
    declarations: [SessionsPage, SessionItemComponent, ContextMenuComponent, SessionSettingsComponent],
})
export class SessionsPageModule {}
