import { AboutPage } from './about.page';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';
import { NgModule } from '@angular/core';
import { SharedComponentModule } from './../../component/shared-component.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SharedComponentModule, MarkdownModule.forChild()],
    declarations: [AboutPage],
})
export class AboutPageModule {}
