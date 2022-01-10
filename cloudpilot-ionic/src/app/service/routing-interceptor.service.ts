import { ActivatedRouteSnapshot, CanActivate, Router, RouterStateSnapshot } from '@angular/router';

import { Injectable } from '@angular/core';
import { LinkApi } from './link-api.service';

@Injectable({ providedIn: 'root' })
export class RoutingInterceptor implements CanActivate {
    constructor(private linkApi: LinkApi, private router: Router) {}

    canActivate(route: ActivatedRouteSnapshot): boolean {
        if (route.url[0]?.path?.startsWith('install')) {
            this.linkApi.dispatchInstallationRequest(route.queryParams.url);
            this.router.navigateByUrl('/tab/emulation');

            return false;
        }

        return true;
    }
}
