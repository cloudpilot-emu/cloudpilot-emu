import sinon from 'sinon';
import assert from 'assert';
import { DynamicTimeout } from '../helper/dynamicTimeout';

describe('dynamic timeout', () => {
    beforeEach(() => {
        sinon.useFakeTimers();
    });

    afterEach(() => {
        sinon.restore();
    });

    it('does not expire for the initial timer has elapsed', () => {
        let expired = false;

        new DynamicTimeout(500, () => (expired = true));

        sinon.clock.tick(499);

        assert(!expired);
    });

    it('does expires once the initial timer has elapsed', () => {
        let expired = false;

        new DynamicTimeout(500, () => (expired = true));

        sinon.clock.tick(500);

        assert(expired);
    });

    it('increasing the timeout postpones expiring the timeout', () => {
        let expired = false;

        const timeout = new DynamicTimeout(500, () => (expired = true));

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

        const timeout = new DynamicTimeout(500, () => (expired = true));

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

        const timeout = new DynamicTimeout(500, () => (expired = true));

        sinon.clock.tick(499);

        timeout.cancel();
        sinon.clock.tick(100);

        assert(!expired);
    });
});
