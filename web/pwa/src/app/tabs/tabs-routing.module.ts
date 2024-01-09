import { RouterModule, Routes } from '@angular/router';

import { AboutPage } from '@pwa/page/about/about.page';
import { EmulationPage } from '@pwa/page/emulation/emulation.page';
import { NgModule } from '@angular/core';
import { SettingsPage } from '@pwa/page/settings/settings.page';
import { StoragePage } from '@pwa/page/storage/storage.page';
import { TabsPage } from './tabs.page';

// We want to use onPush change detection for the sessions page, and this does not work
// if the actual page is top level.
import { NavWrapperComponent as SessionsPage } from '@pwa/page/sessions/nav-wrapper/nav-wrapper.component';

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
