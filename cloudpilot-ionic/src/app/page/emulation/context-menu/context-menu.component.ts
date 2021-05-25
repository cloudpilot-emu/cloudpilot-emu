import { Component, Input, OnInit } from '@angular/core';
import { quirkNoPoweroff, supportsDBExport } from 'src/app/helper/deviceProperties';

import { BackupService } from './../../../service/backup.service';
import { EmulationService } from './../../../service/emulation.service';
import { EmulationStateService } from 'src/app/service/emulation-state.service';

@Component({
    selector: 'app-emulation-context-menu',
    templateUrl: './context-menu.component.html',
    styleUrls: ['./context-menu.component.scss'],
})
export class ContextMenuComponent implements OnInit {
    constructor(
        public emulationService: EmulationService,
        private emulationStateService: EmulationStateService,
        private backupService: BackupService
    ) {}

    ngOnInit(): void {}

    reset(): void {
        this.emulationService.reset();

        this.onClick();
    }

    resetNoExtensions(): void {
        this.emulationService.resetNoExtensions();

        this.onClick();
    }

    resetHard(): void {
        this.emulationService.resetHard();

        this.onClick();
    }

    power(): void {
        this.emulationService.engagePower();

        this.onClick();
    }

    help(): void {
        this.showHelp();

        this.onClick();
    }

    backup(): void {
        this.backupService.saveBackup();

        this.onClick();
    }

    get backupDisabled(): boolean {
        const currentSession = this.emulationStateService.getCurrentSession();

        return (
            !this.emulationService.isRunning() ||
            this.emulationService.isPowerOff() ||
            !this.emulationService.isUiInitialized() ||
            (!!currentSession && !supportsDBExport(currentSession.device))
        );
    }

    get powerDisabled(): boolean {
        const currentSession = this.emulationStateService.getCurrentSession();

        return currentSession ? quirkNoPoweroff(currentSession.device) : false;
    }

    @Input()
    onClick: () => void = () => undefined;

    @Input()
    showHelp: () => void = () => undefined;
}
