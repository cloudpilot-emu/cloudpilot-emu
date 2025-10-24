import { Injectable, Signal, computed, signal } from '@angular/core';
import { Engine } from '@common/engine/Engine';

import { Session } from '@pwa/model/Session';

export interface EmulationContext {
    session: Session;
    engine: Engine;
}

@Injectable({
    providedIn: 'root',
})
export class EmulationContextService {
    constructor() {}

    get session(): Signal<Session | undefined> {
        return this._session;
    }

    get engine(): Signal<Engine | undefined> {
        return this._engine;
    }

    get context(): Signal<EmulationContext | undefined> {
        return this._context;
    }

    setContext(session: Session, engine: Engine): void {
        this._context.set({ session, engine });
    }

    clearContext(): void {
        this._context.set(undefined);
    }

    private _context = signal<EmulationContext | undefined>(undefined);
    private _session = computed(() => this._context()?.session);
    private _engine = computed(() => this._context()?.engine);
}
