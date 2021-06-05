import { AfterViewInit, Component } from '@angular/core';

import { PwaService } from './service/pwa.service';
import { UpdateService } from './service/update.service';

@Component({
    selector: 'app-root',
    templateUrl: 'app.component.html',
    styleUrls: ['app.component.scss'],
})
export class AppComponent implements AfterViewInit {
    constructor(private pwaService: PwaService, private updateService: UpdateService) {}

    ngAfterViewInit(): void {
        this.pwaService.invite();
        this.updateService.start();
    }
}
