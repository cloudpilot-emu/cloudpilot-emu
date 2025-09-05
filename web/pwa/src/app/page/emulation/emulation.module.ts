import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';

import { SharedComponentModule } from '@pwa/component/shared-component.module';

import { BreadcrumbMenuComponent } from './breadcrumb-menu/breadcrumb-menu.component';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EmulationPage } from './emulation.page';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SharedComponentModule],
    declarations: [EmulationPage, ContextMenuComponent, BreadcrumbMenuComponent],
})
export class EmulationPageModule {}
