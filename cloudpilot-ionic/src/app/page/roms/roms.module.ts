import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { RomsPageRoutingModule } from './roms-routing.module';

import { RomsPage } from './roms.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    RomsPageRoutingModule
  ],
  declarations: [RomsPage]
})
export class RomsPageModule {}
