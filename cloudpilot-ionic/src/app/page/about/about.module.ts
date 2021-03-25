import { AboutPage } from './about.page';
import { AboutPageRoutingModule } from './about-routing.module';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SharedComponentModule } from './../../component/shared-component.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, AboutPageRoutingModule, SharedComponentModule],
    declarations: [AboutPage],
})
export class AboutPageModule {}
