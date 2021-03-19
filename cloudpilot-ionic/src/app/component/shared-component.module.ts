import { CommonModule } from '@angular/common';
import { HelpComponent } from './help/help.component';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';
import { NgModule } from '@angular/core';
import { PwaPromptComponent } from './pwa-prompt/pwa-prompt.component';

@NgModule({
    declarations: [HelpComponent, PwaPromptComponent],
    imports: [CommonModule, MarkdownModule.forChild(), IonicModule],
    exports: [HelpComponent, PwaPromptComponent],
})
export class SharedComponentModule {}
