import assert from 'assert';
import sinon from 'sinon';

import { Loader, LoaderAdapter } from '../helper/Loader';

class MockLoaderAdapter implements LoaderAdapter {
    public showCalls: string[] = [];
    public hideCalls: number = 0;
    public showDelay: number = 0;
    public hideDelay: number = 0;
    public shouldThrow: boolean = false;

    async show(message: string): Promise<void> {
        if (this.shouldThrow) throw new Error('Mock adapter error');

        if (this.showDelay > 0) {
            await new Promise((resolve) => setTimeout(resolve, this.showDelay));
        }
        this.showCalls.push(message);
    }

    async hide(): Promise<void> {
        if (this.shouldThrow) throw new Error('Mock adapter error');

        if (this.hideDelay > 0) {
            await new Promise((resolve) => setTimeout(resolve, this.hideDelay));
        }
        this.hideCalls++;
    }

    reset(): void {
        this.showCalls = [];
        this.hideCalls = 0;
        this.shouldThrow = false;
    }
}

describe('Loader', () => {
    let adapter: MockLoaderAdapter;
    let loader: Loader;

    beforeEach(() => {
        sinon.useFakeTimers();
        adapter = new MockLoaderAdapter();
        loader = new Loader(adapter, 100, () => sinon.clock.now); // 100ms hide delay with mocked clock
    });

    afterEach(() => {
        sinon.restore();
    });

    describe('basic functionality', () => {
        it('shows loader immediately with zero delay', async () => {
            await loader.add('Loading...', 0);

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.showCalls[0], 'Loading...');
            assert.strictEqual(adapter.hideCalls, 0);
        });

        it('shows loader after delay', async () => {
            const promise = loader.add('Loading...', 500);

            assert.strictEqual(adapter.showCalls.length, 0);

            sinon.clock.tick(499);
            assert.strictEqual(adapter.showCalls.length, 0);

            sinon.clock.tick(1);
            await promise;

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.showCalls[0], 'Loading...');
        });

        it('hides loader after resolving with hide delay', async () => {
            const handle = await loader.add('Loading...', 0);

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.hideCalls, 0);

            loader.resolve(handle);

            // Should not hide immediately due to hide delay
            assert.strictEqual(adapter.hideCalls, 0);

            sinon.clock.tick(99);
            assert.strictEqual(adapter.hideCalls, 0);

            sinon.clock.tick(1);
            await sinon.clock.tickAsync(0); // Allow async operations to complete
            assert.strictEqual(adapter.hideCalls, 1);
        });

        it('hides loader immediately with zero hide delay', async () => {
            const loaderNoDelay = new Loader(adapter, 0, () => sinon.clock.now);
            const handle = await loaderNoDelay.add('Loading...', 0);

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.hideCalls, 0);

            loaderNoDelay.resolve(handle);

            await sinon.clock.tickAsync(0); // Allow async operations to complete
            assert.strictEqual(adapter.hideCalls, 1);
        });
    });

    describe('stacking behavior', () => {
        it('shows earliest message from unresolved loaders', async () => {
            await loader.add('First', 0);
            await loader.add('Second', 0);
            await loader.add('Third', 0);

            assert.strictEqual(adapter.showCalls.length, 3);
            assert.strictEqual(adapter.showCalls[2], 'First'); // Last call should show first message
        });

        it('updates message when earliest loader is resolved', async () => {
            const handle1 = await loader.add('First', 0);
            await loader.add('Second', 0);

            assert.strictEqual(adapter.showCalls[adapter.showCalls.length - 1], 'First');

            loader.resolve(handle1);
            sinon.clock.tick(0); // Allow any pending operations

            // Should still be showing since second loader is active
            assert.strictEqual(adapter.hideCalls, 0);
        });

        it('handles loaders without messages', async () => {
            await loader.add(undefined, 0);
            await loader.add('With message', 0);

            assert.strictEqual(adapter.showCalls[adapter.showCalls.length - 1], 'With message');
        });

        it('shows empty string when no loader has a message', async () => {
            await loader.add(undefined, 0);
            await loader.add(undefined, 0);

            assert.strictEqual(adapter.showCalls[adapter.showCalls.length - 1], '');
        });
    });

    describe('delay management', () => {
        it('allows reducing delay with successive loaders', async () => {
            const promise1 = loader.add('First', 1000);

            sinon.clock.tick(500);
            assert.strictEqual(adapter.showCalls.length, 0);

            const promise2 = loader.add('Second', 200); // Should reschedule to show earlier

            sinon.clock.tick(200);
            await Promise.all([promise1, promise2]);

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.showCalls[0], 'First'); // First message wins
        });

        it('does not increase delay with successive loaders', async () => {
            const promise1 = loader.add('First', 200);

            sinon.clock.tick(100);
            assert.strictEqual(adapter.showCalls.length, 0);

            const promise2 = loader.add('Second', 1000); // Should not reschedule

            sinon.clock.tick(100);
            await Promise.all([promise1, promise2]);

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.showCalls[0], 'First');
        });

        it('cancels hide timeout when new loader is added', async () => {
            const handle1 = await loader.add('First', 0);
            loader.resolve(handle1);

            // Hide should be scheduled
            sinon.clock.tick(50);
            assert.strictEqual(adapter.hideCalls, 0);

            // Add new loader before hide timeout
            await loader.add('Second', 0);

            sinon.clock.tick(100);
            assert.strictEqual(adapter.hideCalls, 0); // Hide should be cancelled
        });
    });

    describe('showWhile method', () => {
        it('shows loader during async operation', async () => {
            let operationComplete = false;

            const promise = loader.showWhile(
                async () => {
                    assert.strictEqual(adapter.showCalls.length, 1);
                    assert.strictEqual(adapter.showCalls[0], 'Working...');

                    operationComplete = true;
                    return 'result';
                },
                'Working...',
                0,
            );

            const result = await promise;

            assert.strictEqual(result, 'result');
            assert(operationComplete);

            // Should schedule hide after operation completes
            sinon.clock.tick(100);
            await sinon.clock.tickAsync(0); // Allow async operations to complete
            assert.strictEqual(adapter.hideCalls, 1);
        });

        it('resolves loader even if operation throws', async () => {
            let errorThrown = false;

            try {
                await loader.showWhile(
                    async () => {
                        throw new Error('Operation failed');
                    },
                    'Working...',
                    0,
                );
            } catch (error) {
                errorThrown = true;
                assert.strictEqual((error as Error).message, 'Operation failed');
            }

            assert(errorThrown);

            // Should still hide the loader
            sinon.clock.tick(100);
            await sinon.clock.tickAsync(0); // Allow async operations to complete
            assert.strictEqual(adapter.hideCalls, 1);
        });
    });

    describe('error handling', () => {
        it('throws error on double resolve', async () => {
            const handle = await loader.add('Loading...', 0);

            loader.resolve(handle);

            assert.throws(() => {
                loader.resolve(handle);
            }, /double resolve loader/);
        });

        it('throws error when resolving non-existent handle', () => {
            assert.throws(() => {
                loader.resolve(999);
            }, /double resolve loader/);
        });

        it('logs warning for out-of-order resolution', async () => {
            const consoleErrorStub = sinon.stub(console, 'error');

            const handle1 = await loader.add('First', 0);
            await loader.add('Second', 0);

            // Resolve first handle (out of order)
            loader.resolve(handle1);

            assert(consoleErrorStub.calledWith('BUG: loader resolved out of order'));

            consoleErrorStub.restore();
        });
    });

    describe('cleanup behavior', () => {
        it('removes resolved loaders from stack', async () => {
            const handle1 = await loader.add('First', 0);
            const handle2 = await loader.add('Second', 0);
            const handle3 = await loader.add('Third', 0);

            // Resolve in reverse order (LIFO)
            loader.resolve(handle3);
            loader.resolve(handle2);
            loader.resolve(handle1);

            // All should be cleaned up, hide should be scheduled
            sinon.clock.tick(100);
            await sinon.clock.tickAsync(0); // Allow async operations to complete
            assert.strictEqual(adapter.hideCalls, 1);
        });

        it('only removes resolved loaders from end of stack', async () => {
            const handle1 = await loader.add('First', 0);
            const handle2 = await loader.add('Second', 0);
            const handle3 = await loader.add('Third', 0);

            // Resolve middle loader first
            loader.resolve(handle2);

            // Should not hide yet since handle3 and handle1 are still active
            sinon.clock.tick(100);
            assert.strictEqual(adapter.hideCalls, 0);

            // Resolve last loader
            loader.resolve(handle3);

            // Still should not hide since handle1 is active
            sinon.clock.tick(100);
            assert.strictEqual(adapter.hideCalls, 0);

            // Resolve first loader
            loader.resolve(handle1);

            // Now should hide
            sinon.clock.tick(100);
            await sinon.clock.tickAsync(0); // Allow async operations to complete
            assert.strictEqual(adapter.hideCalls, 1);
        });
    });

    describe('concurrency and mutex behavior', () => {
        it('serializes adapter calls with mutex', async () => {
            const promise1 = loader.add('First', 0);
            const promise2 = loader.add('Second', 0);

            await Promise.all([promise1, promise2]);

            // Should have made multiple show calls but with consistent message
            assert(adapter.showCalls.length >= 1);
            assert.strictEqual(adapter.showCalls[0], 'First');
        });

        it('handles adapter errors gracefully in show', async () => {
            adapter.shouldThrow = true;

            try {
                await loader.add('Loading...', 0);
                // The error should be thrown from the adapter
                assert.fail('Expected error to be thrown');
            } catch (error) {
                assert.strictEqual((error as Error).message, 'Mock adapter error');
            }
        });
    });

    describe('edge cases', () => {
        it('handles rapid add/resolve cycles', async () => {
            for (let i = 0; i < 10; i++) {
                const handle = await loader.add(`Loading ${i}`, 0);
                loader.resolve(handle);
            }

            // Should end up hidden
            sinon.clock.tick(100);
            await sinon.clock.tickAsync(0); // Allow async operations to complete
            assert.strictEqual(adapter.hideCalls, 1);
        });

        it('handles zero and negative delays correctly', async () => {
            await loader.add('Immediate', 0);
            await loader.add('Negative', -100);

            assert.strictEqual(adapter.showCalls.length, 2);
        });

        it('maintains correct state when show is scheduled but not yet executed', async () => {
            const promise = loader.add('Delayed', 500);
            const handle = await promise;

            // Resolve before show executes
            loader.resolve(handle);

            // Show should still execute but then immediately schedule hide
            sinon.clock.tick(500);

            assert.strictEqual(adapter.showCalls.length, 0); // Should not show since resolved

            sinon.clock.tick(100);
            assert.strictEqual(adapter.hideCalls, 0); // Should not hide since never showed
        });
    });
});
