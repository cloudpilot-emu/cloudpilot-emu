import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { MarkdownModule } from 'ngx-markdown';

import { BreadcrumbComponent } from './breadcrumb/breadcrumb.component';
import { CredentialsPromptComponent } from './credentials-prompt/credentials-prompt.component';
import { AutofocusDirective } from './directive/autofocus.directive';
import { HelpComponent } from './help/help.component';
import { CardSizePipe } from './pipe/card-size.pipe';
import { FileSizePipe } from './pipe/file-size.pipe';
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
        AutofocusDirective,
        BreadcrumbComponent,
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
        AutofocusDirective,
        BreadcrumbComponent,
    ],
})
export class SharedComponentModule {}
