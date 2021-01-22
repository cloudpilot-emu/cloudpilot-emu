import { CommonModule } from '@angular/common';
import { EmulationPage } from './emulation.page';
import { EmulationPageRoutingModule } from './emulation-routing.module';
import { FormsModule } from '@angular/forms';
import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';

@NgModule({
    imports: [CommonModule, FormsModule, IonicModule, EmulationPageRoutingModule],
    declarations: [EmulationPage],
})
export class EmulationPageModule {}
