import { ActivatedRouteSnapshot, CanActivate, Router } from '@angular/router';

import { Injectable } from '@angular/core';
import { LinkApi } from './link-api.service';

@Injectable({ providedIn: 'root' })
export class RoutingInterceptor implements CanActivate {
    constructor(private linkApi: LinkApi, private router: Router) {}

    canActivate(route: ActivatedRouteSnapshot): boolean {
        if (route.url[0]?.path?.startsWith('install')) {
            this.linkApi.installation.dispatchRequest(route.queryParams.url);
            this.router.navigateByUrl('/tab/emulation');

            return false;
        }

        if (route.url[0]?.path?.startsWith('import')) {
            this.linkApi.import.dispatchRequest(route.queryParams.url);
            this.router.navigateByUrl('/tab/sessions');

            return false;
        }

        return true;
    }
}
