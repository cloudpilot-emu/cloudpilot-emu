import { RouterModule, Routes } from '@angular/router';

import { AboutPage } from './../page/about/about.page';
import { EmulationPage } from './../page/emulation/emulation.page';
import { NgModule } from '@angular/core';
import { SessionsPage } from './../page/sessions/sessions.page';
import { SettingsPage } from './../page/settings/settings.page';
import { TabsPage } from './tabs.page';

const routes: Routes = [
    {
        path: 'tab',
        component: TabsPage,
        children: [
            {
                path: 'emulation',
                component: EmulationPage,
            },
            {
                path: 'sessions',
                component: SessionsPage,
            },
            {
                path: 'settings',
                component: SettingsPage,
            },
            {
                path: 'about',
                component: AboutPage,
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
