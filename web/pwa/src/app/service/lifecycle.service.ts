import { Injectable } from '@angular/core';

@Injectable({ providedIn: 'root' })
export class LifecycleService {
    constructor() {
        this.appInitialized = new Promise<void>((resolve) => (this.onInitialize = resolve));
    }

    notifyAppIsInitialized(): void {
        this.onInitialize();
    }

    appIsInitialized(): Promise<void> {
        return this.appInitialized;
    }

    private appInitialized: Promise<void>;
    private onInitialize: () => void = () => undefined;
}
