import { CommonModule } from '@angular/common';
import { ContextMenuComponent } from './context-menu/context-menu.component';
import { EmulationPage } from './emulation.page';
import { EmulationPageRoutingModule } from './emulation-routing.module';
import { FormsModule } from '@angular/forms';
import { HelpComponent } from './../../component/help/help.component';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, EmulationPageRoutingModule],
    declarations: [EmulationPage, ContextMenuComponent, HelpComponent],
})
export class EmulationPageModule {}
