import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { RomsPage } from './roms.page';

const routes: Routes = [
  {
    path: '',
    component: RomsPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class RomsPageRoutingModule {}
