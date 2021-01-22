import { RouterModule, Routes } from '@angular/router';

import { EmulationPage } from './emulation.page';
import { NgModule } from '@angular/core';

const routes: Routes = [
    {
        path: '',
        component: EmulationPage,
    },
];

@NgModule({
    imports: [RouterModule.forChild(routes)],
    exports: [RouterModule],
})
export class EmulationPageRoutingModule {}
