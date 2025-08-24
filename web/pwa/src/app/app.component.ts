import { AfterViewInit, Component } from '@angular/core';

import { BootstrapService } from '@pwa/service/bootstrap-service';
import { PwaService } from '@pwa/service/pwa.service';
import { UpdateService } from '@pwa/service/update.service';
import { InfoService } from './service/info.service';

@Component({
    selector: 'app-root',
    templateUrl: 'app.component.html',
    styleUrls: ['app.component.scss'],
    standalone: false
})
export class AppComponent implements AfterViewInit {
    constructor(
        private pwaService: PwaService,
        private updateService: UpdateService,
        private bootstrapService: BootstrapService,
        private infoService: InfoService,
    ) {}

    async ngAfterViewInit(): Promise<void> {
        this.bootstrapService.notifyHasRendered();

        await this.infoService.start();

        this.pwaService.invite();
        this.updateService.start();
    }
}
