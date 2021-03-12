import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { HelpComponent } from '../../component/help/help.component';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';
import { NgModule } from '@angular/core';
import { SessionItemComponent } from './session-item/session-item.component';
import { SessionSettingsComponent } from './settings/settings.component';
import { SessionsPage } from './sessions.page';
import { SessionsPageRoutingModule } from './sessions-routing.module';

@NgModule({
    imports: [
        CommonModule,
        FormsModule,
        IonicModule,
        SessionsPageRoutingModule,
        ReactiveFormsModule,
        MarkdownModule.forChild(),
    ],
    declarations: [SessionsPage, SessionItemComponent, ContextMenuComponent, SessionSettingsComponent, HelpComponent],
})
export class SessionsPageModule {}
