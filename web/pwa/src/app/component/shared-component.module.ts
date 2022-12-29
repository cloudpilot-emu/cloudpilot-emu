import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { CardSizePipe } from './pipe/card-size.pipe';
import { CommonModule } from '@angular/common';
import { CredentialsPromptComponent } from './credentials-prompt/credentials-prompt.component';
import { FileSizePipe } from './pipe/file-size.pipe';
import { HelpComponent } from './help/help.component';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';
import { NgModule } from '@angular/core';
import { PwaPromptComponent } from './pwa-prompt/pwa-prompt.component';
import { RemoteUrlPromptComponent } from './remote-url-prompt/remote-url-prompt.component';
import { SessionSettingsComponent } from './session-settings/session-settings.component';

@NgModule({
    declarations: [
        HelpComponent,
        PwaPromptComponent,
        CredentialsPromptComponent,
        RemoteUrlPromptComponent,
        SessionSettingsComponent,
        CardSizePipe,
        FileSizePipe,
    ],
    imports: [CommonModule, MarkdownModule.forChild(), IonicModule, FormsModule, ReactiveFormsModule],
    exports: [
        HelpComponent,
        PwaPromptComponent,
        CredentialsPromptComponent,
        RemoteUrlPromptComponent,
        SessionSettingsComponent,
        CardSizePipe,
        FileSizePipe,
    ],
})
export class SharedComponentModule {}
