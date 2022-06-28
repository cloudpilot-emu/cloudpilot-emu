import { Event } from 'microevent.ts';

export class Watcher<T> {
    constructor(private query: () => T) {
        this.last = query();
    }

    update(): void {
        const current = this.query();

        if (current !== this.last) {
            this.changeEvent.dispatch(current);
            this.last = current;
        }
    }

    readonly changeEvent = new Event<T>();

    private last: T;
}
