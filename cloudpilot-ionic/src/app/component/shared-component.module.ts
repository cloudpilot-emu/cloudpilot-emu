import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CommonModule } from '@angular/common';
import { CredentialsPromptComponent } from './credentials-prompt/credentials-prompt.component';
import { HelpComponent } from './help/help.component';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';
import { NgModule } from '@angular/core';
import { PwaPromptComponent } from './pwa-prompt/pwa-prompt.component';
import { RemoteUrlPromptComponent } from './remote-url-prompt/remote-url-prompt.component';

@NgModule({
    declarations: [HelpComponent, PwaPromptComponent, CredentialsPromptComponent, RemoteUrlPromptComponent],
    imports: [CommonModule, MarkdownModule.forChild(), IonicModule, FormsModule, ReactiveFormsModule],
    exports: [HelpComponent, PwaPromptComponent, CredentialsPromptComponent, RemoteUrlPromptComponent],
})
export class SharedComponentModule {}
