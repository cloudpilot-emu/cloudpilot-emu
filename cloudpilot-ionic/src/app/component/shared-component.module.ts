import { CommonModule } from '@angular/common';
import { HelpComponent } from './help/help.component';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';
import { NgModule } from '@angular/core';

@NgModule({
    declarations: [HelpComponent],
    imports: [CommonModule, MarkdownModule.forChild(), IonicModule],
})
export class SharedComponentModule {}
