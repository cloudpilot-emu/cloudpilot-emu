import assert from 'assert';
import { Event } from 'microevent.ts';
import sinon from 'sinon';

import { DynamicTimeout } from '../helper/dynamicTimeout';

class DocumentMock {
    constructor(public visibilityState: DocumentVisibilityState) {}

    addEventListener(event: string, handler: () => void): void {
        this.visibilityChangeEvent.addHandler(handler);
    }

    removeEventListener(event: string, handler: () => void): void {
        this.visibilityChangeEvent.removeHandler(handler);
    }

    setVisibilityState(visibilityState: DocumentVisibilityState) {
        if (visibilityState === this.visibilityState) return;

        this.visibilityState = visibilityState;
        this.visibilityChangeEvent.dispatch(undefined);
    }

    visibilityChangeEvent = new Event();
}

describe('dynamic timeout', () => {
    let document!: DocumentMock;

    beforeEach(() => {
        sinon.useFakeTimers();
        document = new DocumentMock('visible');
    });

    afterEach(() => {
        sinon.restore();
    });

    it('does not expire if the initial timer has elapsed', () => {
        let expired = false;

        new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(499);

        assert(!expired);
    });

    it('does expires once the initial timer has elapsed', () => {
        let expired = false;

        new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(500);

        assert(expired);
    });

    it('does not expires if it was not started', () => {
        let expired = false;

        new DynamicTimeout(500, () => (expired = true), document);

        sinon.clock.tick(500);

        assert(!expired);
    });

    it('increasing the timeout postpones expiring the timeout', () => {
        let expired = false;

        const timeout = new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(499);
        assert(!expired);

        timeout.increase(100);
        sinon.clock.tick(100);
        assert(!expired);

        sinon.clock.tick(1);
        assert(expired);
    });

    it('increasing the timeout multiple times expiring the timeout', () => {
        let expired = false;

        const timeout = new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(499);
        assert(!expired);

        timeout.increase(100);
        sinon.clock.tick(100);
        assert(!expired);

        timeout.increase(100);
        sinon.clock.tick(100);
        assert(!expired);

        sinon.clock.tick(1);
        assert(expired);
    });

    it('cancel aborts the timeout', () => {
        let expired = false;

        const timeout = new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(499);

        timeout.cancel();
        sinon.clock.tick(100);

        assert(!expired);
    });

    it('deregisters timers once it has elapsed', () => {
        new DynamicTimeout(500, () => undefined, document).start();

        sinon.clock.tick(500);

        assert(!document.visibilityChangeEvent.hasHandlers);
    });

    it('deregisters timers if it is aborted', () => {
        const timeout = new DynamicTimeout(500, () => undefined, document).start();

        sinon.clock.tick(499);

        timeout.cancel();

        assert(!document.visibilityChangeEvent.hasHandlers);
    });

    it('pauses if the document becomes invisible', () => {
        let expired = false;

        new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(250);

        document.setVisibilityState('hidden');
        sinon.clock.tick(300);

        assert(!expired);

        document.setVisibilityState('visible');

        sinon.clock.tick(249);
        assert(!expired);

        sinon.clock.tick(1);
        assert(expired);
    });

    it('sets isTainted if paused by visibility change', () => {
        let expired = false;

        const timer = new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(250);

        document.setVisibilityState('hidden');
        document.setVisibilityState('visible');

        sinon.clock.tick(250);
        assert(expired);
        assert(timer.isTainted());
    });

    it('does not set isTainted if not paused by visibility change', () => {
        let expired = false;

        const timer = new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(500);
        assert(expired);
        assert(!timer.isTainted());
    });

    it('tracks the accumulated timeout', () => {
        let expired = false;

        const timer = new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(250);

        document.setVisibilityState('hidden');
        sinon.clock.tick(300);

        document.setVisibilityState('visible');
        sinon.clock.tick(50);

        document.setVisibilityState('hidden');
        sinon.clock.tick(300);

        timer.increase(100);

        document.setVisibilityState('visible');
        sinon.clock.tick(299);
        assert(!expired);

        sinon.clock.tick(500);
        assert(expired);
        assert(timer.getAccumulatedTimeout() === 600);
    });

    it('can be canceled while paused', () => {
        let expired = false;

        const timer = new DynamicTimeout(500, () => (expired = true), document).start();

        sinon.clock.tick(250);

        document.setVisibilityState('hidden');

        timer.cancel();
        sinon.clock.tick(500);

        assert(!expired);
    });
});
