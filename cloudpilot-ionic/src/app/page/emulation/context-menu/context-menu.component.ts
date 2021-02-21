import { Component, Input, OnInit } from '@angular/core';

import { EmulationService } from './../../../service/emulation.service';

@Component({
    selector: 'app-emulation-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
})
export class ContextMenuComponent implements OnInit {
    constructor(public emulationService: EmulationService) {}

    ngOnInit() {}

    reset() {
        this.emulationService.reset();

        this.onClick();
    }

    resetNoExtensions() {
        this.emulationService.resetNoExtensions();

        this.onClick();
    }

    resetHard() {
        this.emulationService.resetHard();

        this.onClick();
    }

    @Input()
    onClick: () => void = () => undefined;
}
