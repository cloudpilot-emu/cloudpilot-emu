import assert from 'assert';
import sinon from 'sinon';

import { Throttle } from '../helper/Throttle';

describe('throttle', () => {
    let now: number;
    let timer: () => number;

    beforeEach(() => {
        sinon.useFakeTimers();
        now = 0;
        timer = () => now;
    });

    afterEach(() => {
        sinon.restore();
    });

    it('first trigger is deferred', () => {
        let calls = 0;

        const throttle = new Throttle(() => calls++, 100, timer);
        throttle.trigger();

        assert.strictEqual(calls, 0);
    });

    it('deferred execution fires after timeout', () => {
        let calls = 0;

        const throttle = new Throttle(() => calls++, 100, timer);
        throttle.trigger();

        sinon.clock.tick(100);

        assert.strictEqual(calls, 1);
    });

    it('triggers while timer pending are dropped', () => {
        let calls = 0;

        const throttle = new Throttle(() => calls++, 100, timer);
        throttle.trigger();
        throttle.trigger();
        throttle.trigger();

        sinon.clock.tick(100);

        assert.strictEqual(calls, 1);
    });

    it('after cooldown, next trigger executes immediately', () => {
        let calls = 0;

        const throttle = new Throttle(() => calls++, 100, timer);
        throttle.trigger();

        now = 100;
        sinon.clock.tick(100);
        assert.strictEqual(calls, 1);

        now = 201;
        throttle.trigger();
        assert.strictEqual(calls, 2);
    });

    it('fn exceptions do not break future triggers', () => {
        let calls = 0;

        const throttle = new Throttle(
            () => {
                calls++;
                if (calls === 1) throw new Error('boom');
            },
            100,
            timer,
        );

        throttle.trigger();

        now = 100;
        assert.throws(() => sinon.clock.tick(100));
        assert.strictEqual(calls, 1);

        now = 201;
        throttle.trigger();
        assert.strictEqual(calls, 2);
    });
});
