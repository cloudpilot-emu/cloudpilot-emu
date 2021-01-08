import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { EmulationPage } from './emulation.page';

const routes: Routes = [
  {
    path: '',
    component: EmulationPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class EmulationPageRoutingModule {}
