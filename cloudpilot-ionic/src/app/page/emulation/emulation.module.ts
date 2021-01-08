import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { EmulationPageRoutingModule } from './emulation-routing.module';

import { EmulationPage } from './emulation.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    EmulationPageRoutingModule
  ],
  declarations: [EmulationPage]
})
export class EmulationPageModule {}
