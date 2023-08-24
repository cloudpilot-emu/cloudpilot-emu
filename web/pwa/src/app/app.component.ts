import { AfterViewInit, Component } from '@angular/core';

import { BootstrapService } from '@pwa/service/bootstrap-service';
import { PwaService } from '@pwa/service/pwa.service';
import { UpdateService } from '@pwa/service/update.service';

@Component({
    selector: 'app-root',
    templateUrl: 'app.component.html',
    styleUrls: ['app.component.scss'],
})
export class AppComponent implements AfterViewInit {
    constructor(
        private pwaService: PwaService,
        private updateService: UpdateService,
        private bootstrapService: BootstrapService,
    ) {}

    ngAfterViewInit(): void {
        this.bootstrapService.notifyHasRendered();

        this.pwaService.invite();
        this.updateService.start();
    }
}
