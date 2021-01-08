import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SessionsPage } from './sessions.page';
import { SessionsPageRoutingModule } from './sessions-routing.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SessionsPageRoutingModule],
    declarations: [SessionsPage],
})
export class SessionsPageModule {}
