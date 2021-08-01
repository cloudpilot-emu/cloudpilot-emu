import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { CredentialsPromptComponent } from './credentials-prompt/credentials-prompt.component';
import { HelpComponent } from './help/help.component';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';
import { NgModule } from '@angular/core';
import { PwaPromptComponent } from './pwa-prompt/pwa-prompt.component';

@NgModule({
    declarations: [HelpComponent, PwaPromptComponent, CredentialsPromptComponent],
    imports: [CommonModule, MarkdownModule.forChild(), IonicModule, FormsModule, ReactiveFormsModule],
    exports: [HelpComponent, PwaPromptComponent, CredentialsPromptComponent],
})
export class SharedComponentModule {}
