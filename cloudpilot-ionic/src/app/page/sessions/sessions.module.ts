import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SessionItemComponent } from './session-item/session-item.component';
import { SessionsPage } from './sessions.page';
import { SessionsPageRoutingModule } from './sessions-routing.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SessionsPageRoutingModule],
    declarations: [SessionsPage, SessionItemComponent],
})
export class SessionsPageModule {}
