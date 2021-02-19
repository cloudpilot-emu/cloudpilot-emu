import { Cloudpilot } from '../helper/Cloudpilot';
import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class EmulationService {
    switchSession(id: number) {
        this.currentSession = id;
    }

    getCurrentSession(): number {
        return this.currentSession;
    }

    readonly cloudpilot = Cloudpilot.create();

    private currentSession = -1;
}
