import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';

import { SharedComponentModule } from '@pwa/component/shared-component.module';

import { AboutPage } from './about.page';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SharedComponentModule, MarkdownModule.forChild()],
    declarations: [AboutPage],
})
export class AboutPageModule {}
