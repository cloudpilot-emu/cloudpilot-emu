import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EmulationPage } from './emulation.page';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { SharedComponentModule } from './../../component/shared-component.module';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, SharedComponentModule],
    declarations: [EmulationPage, ContextMenuComponent],
})
export class EmulationPageModule {}
