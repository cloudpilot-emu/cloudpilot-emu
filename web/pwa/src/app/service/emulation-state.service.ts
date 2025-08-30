import { Injectable, Signal, signal, WritableSignal } from '@angular/core';
import { Session } from '@pwa/model/Session';

@Injectable({
    providedIn: 'root',
})
export class EmulationStateService {
    constructor() {}

    get currentSession(): Signal<Session | undefined> {
        return this._currentSession;
    }

    setCurrentSession(session: Session | undefined): void {
        this._currentSession.set(session);
    }

    private _currentSession: WritableSignal<Session | undefined> = signal(undefined);
}
