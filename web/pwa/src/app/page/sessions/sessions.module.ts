import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';

import { SharedComponentModule } from '@pwa/component/shared-component.module';

import { ActionMenuComponent } from './action-menu/action-menu.component';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { NavWrapperComponent } from './nav-wrapper/nav-wrapper.component';
import { SessionItemComponent } from './session-item/session-item.component';
import { SessionsPage } from './sessions.page';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, ReactiveFormsModule, SharedComponentModule],
    declarations: [SessionsPage, SessionItemComponent, ContextMenuComponent, NavWrapperComponent, ActionMenuComponent],
})
export class SessionsPageModule {}
