import { Injectable } from '@angular/core';

@Injectable({ providedIn: 'root' })
export class BootstrapService {
    constructor() {
        this.hasRenderedPromise = new Promise((resolve) => (this.hasRenderedResolve = resolve));
    }

    notifyHasRendered(): void {
        this.hasRenderedResolve();
    }

    hasRendered(): Promise<void> {
        return this.hasRenderedPromise;
    }

    private hasRenderedPromise: Promise<void>;
    private hasRenderedResolve!: () => void;
}
