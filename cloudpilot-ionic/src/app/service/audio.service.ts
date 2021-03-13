import { EmulationService } from './emulation.service';
import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class AudioService {
    constructor(private emulationService: EmulationService) {
        this.initialize();
    }

    private async initialize(): Promise<void> {
        const cloudpilot = await this.emulationService.cloudpilot;

        cloudpilot.pwmUpdateEvent.addHandler(({ frequency, dutyCycle }) =>
            console.log(`PWM update: f = ${frequency} Hz, dutyCycle = ${dutyCycle}`)
        );
    }
}
