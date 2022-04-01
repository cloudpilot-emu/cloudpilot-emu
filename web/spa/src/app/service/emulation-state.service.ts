import { Injectable } from '@angular/core';
import { Session } from '../model/Session';

@Injectable({
    providedIn: 'root',
})
export class EmulationStateService {
    constructor() {}

    getCurrentSession(): Session | undefined {
        return this.currentSession;
    }

    setCurrentSession(session: Session | undefined): void {
        this.currentSession = session;
    }

    private currentSession: Session | undefined;
}
