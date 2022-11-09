import { RouterModule, Routes } from '@angular/router';

import { AboutPage } from '@pwa/page/about/about.page';
import { EmulationPage } from '@pwa/page/emulation/emulation.page';
import { NgModule } from '@angular/core';
import { SessionsPage } from '@pwa/page/sessions/sessions.page';
import { SettingsPage } from '@pwa/page/settings/settings.page';
import { StoragePage } from './../page/storage/storage.page';
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
                path: 'storage',
                component: StoragePage,
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
