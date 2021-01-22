import { RouterModule, Routes } from '@angular/router';

import { NgModule } from '@angular/core';
import { SessionsPage } from './sessions.page';

const routes: Routes = [
    {
        path: '',
        component: SessionsPage,
    },
];

@NgModule({
    imports: [RouterModule.forChild(routes)],
    exports: [RouterModule],
})
export class SessionsPageRoutingModule {}
