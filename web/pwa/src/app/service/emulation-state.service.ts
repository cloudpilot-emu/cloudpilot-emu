import { Injectable } from '@angular/core';
import { Session } from '@pwa/model/Session';

@Injectable({
    providedIn: 'root',
})
export class EmulationStateService {
    constructor() {}

    getCurrentSession(): Session | undefined {
        return this.currentSession;
    }

    setCurrentSession(session: Session | undefined): void {
        if (session?.id !== this.currentSession?.id) {
            this.mountedCard = undefined;
        }

        this.currentSession = session;
    }

    setMountedCard(cardId: number): void {
        this.mountedCard = cardId;
    }

    clearMountedCard(): void {
        this.mountedCard = undefined;
    }

    hasMountedCard(): boolean {
        return this.mountedCard !== undefined;
    }

    getMountedCard(): number | undefined {
        return this.mountedCard;
    }

    private currentSession: Session | undefined;
    private mountedCard: number | undefined;
}
