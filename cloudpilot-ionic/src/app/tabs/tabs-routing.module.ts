import { RouterModule, Routes } from '@angular/router';

import { NgModule } from '@angular/core';
import { TabsPage } from './tabs.page';

const routes: Routes = [
    {
        path: 'tab',
        component: TabsPage,
        children: [
            {
                path: 'emulation',
                loadChildren: () => import('../page/emulation/emulation.module').then((m) => m.EmulationPageModule),
            },
            {
                path: 'sessions',
                loadChildren: () => import('../page/sessions/sessions.module').then((m) => m.SessionsPageModule),
            },
            {
                path: 'settings',
                loadChildren: () => import('../page/settings/settings.module').then((m) => m.SettingsPageModule),
            },
            {
                path: 'about',
                loadChildren: () => import('../page/about/about.module').then((m) => m.AboutPageModule),
            },
            {
                path: '',
                redirectTo: '/tab/sessions',
                pathMatch: 'full',
            },
        ],
    },
    {
        path: '',
        redirectTo: '/tab/sessions',
        pathMatch: 'full',
    },
];

@NgModule({
    imports: [RouterModule.forChild(routes)],
    exports: [RouterModule],
})
export class TabsPageRoutingModule {}
