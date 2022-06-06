import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SessionItemComponent } from './session-item/session-item.component';
import { SessionsPage } from './sessions.page';
import { SharedComponentModule } from '@pwa/component/shared-component.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, ReactiveFormsModule, SharedComponentModule],
    declarations: [SessionsPage, SessionItemComponent, ContextMenuComponent],
})
export class SessionsPageModule {}
