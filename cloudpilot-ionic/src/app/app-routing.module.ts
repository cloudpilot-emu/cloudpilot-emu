import { HashLocationStrategy, LocationStrategy } from '@angular/common';
import { Injectable, NgModule } from '@angular/core';
import { PreloadAllModules, RouterModule, Routes } from '@angular/router';

@Injectable()
class ReplaceLocationStrategy extends HashLocationStrategy {
    pushState(state: any, title: string, url: string, query: string): void {
        this.replaceState(state, title, url, query);
    }
}

const routes: Routes = [
    {
        path: '',
        loadChildren: () => import('./tabs/tabs.module').then((m) => m.TabsPageModule),
    },
];
@NgModule({
    imports: [RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules, useHash: true })],
    exports: [RouterModule],
    providers: [{ provide: LocationStrategy, useClass: ReplaceLocationStrategy }],
})
export class AppRoutingModule {}
