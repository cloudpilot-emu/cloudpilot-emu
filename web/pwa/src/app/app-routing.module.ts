import { HashLocationStrategy, LocationStrategy } from '@angular/common';
import { Injectable, NgModule } from '@angular/core';
import { PreloadAllModules, RouterModule, Routes } from '@angular/router';

import { DummyComponent } from './component/dummy/dummy.component';
import { RoutingInterceptor } from './service/routing-interceptor.service';
import { TabsPage } from './tabs/tabs.page';
import { TabsPageModule } from './tabs/tabs.module';

@Injectable()
class ReplaceLocationStrategy extends HashLocationStrategy {
    pushState(state: any, title: string, url: string, query: string): void {
        this.replaceState(state, title, url, query);
    }
}

const routes: Routes = [
    {
        path: 'install',
        component: DummyComponent,
        canActivate: [RoutingInterceptor],
    },
    {
        path: 'import',
        component: DummyComponent,
        canActivate: [RoutingInterceptor],
    },
    {
        path: '',
        component: TabsPage,
    },
];
@NgModule({
    imports: [RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules, useHash: true }), TabsPageModule],
    exports: [RouterModule],
    providers: [{ provide: LocationStrategy, useClass: ReplaceLocationStrategy }],
})
export class AppRoutingModule {}
