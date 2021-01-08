import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { TabsPage } from './tabs.page';

const routes: Routes = [
  {
    path: 'tab',
    component: TabsPage,
    children: [
      {
        path: 'emulation',
        loadChildren: () => import('../page/emulation/emulation.module').then(m => m.EmulationPageModule)
      },
      {
        path: 'sessions',
        loadChildren: () => import('../page/sessions/sessions.module').then(m => m.SessionsPageModule)
      },
      {
        path: 'roms',
        loadChildren: () => import('../page/roms/roms.module').then(m => m.RomsPageModule)
      },
      {
        path: 'settings',
        loadChildren: () => import('../page/settings/settings.module').then(m => m.SettingsPageModule)
      },
      {
        path: '',
        redirectTo: '/tab/emulation',
        pathMatch: 'full'
      }
    ]
  },
  {
    path: '',
    redirectTo: '/tab/emulation',
    pathMatch: 'full'
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class TabsPageRoutingModule { }
