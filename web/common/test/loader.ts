import assert from 'assert';
import sinon from 'sinon';

import { Loader, LoaderAdapter } from '../helper/Loader';

interface TestOptions {
    priority?: number;
    theme?: string;
}

class MockLoaderAdapter implements LoaderAdapter<TestOptions> {
    public showCalls: Array<{ message: string | undefined; options: TestOptions | undefined }> = [];
    public hideCalls: number = 0;
    public showDelay: number = 0;
    public hideDelay: number = 0;
    public shouldThrow: boolean = false;

    async show(message: string | undefined, loaderOptions?: TestOptions): Promise<void> {
        if (this.shouldThrow) throw new Error('Mock adapter error');

        if (this.showDelay > 0) {
            await new Promise((resolve) => setTimeout(resolve, this.showDelay));
        }
        this.showCalls.push({ message, options: loaderOptions });
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

    getLastMessage(): string | undefined {
        return this.showCalls.length > 0 ? this.showCalls[this.showCalls.length - 1].message : undefined;
    }

    getLastOptions(): TestOptions | undefined {
        return this.showCalls.length > 0 ? this.showCalls[this.showCalls.length - 1].options : undefined;
    }
}

describe('Loader', () => {
    let adapter: MockLoaderAdapter;
    let loader: Loader<TestOptions>;

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
            await loader.add('Loading…', 0);

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.getLastMessage(), 'Loading…');
            assert.strictEqual(adapter.hideCalls, 0);
        });

        it('shows loader after delay', async () => {
            const promise = loader.add('Loading…', 500);

            assert.strictEqual(adapter.showCalls.length, 0);

            sinon.clock.tick(499);
            assert.strictEqual(adapter.showCalls.length, 0);

            sinon.clock.tick(1);
            await promise;

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.getLastMessage(), 'Loading…');
        });

        it('hides loader after resolving with hide delay', async () => {
            const handle = await loader.add('Loading…', 0);

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
            const handle = await loaderNoDelay.add('Loading…', 0);

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
            assert.strictEqual(adapter.getLastMessage(), 'First'); // Last call should show first message
        });

        it('updates message when earliest loader is resolved', async () => {
            const handle1 = await loader.add('First', 0);
            await loader.add('Second', 0);

            assert.strictEqual(adapter.getLastMessage(), 'First');

            loader.resolve(handle1);
            sinon.clock.tick(0); // Allow any pending operations

            // Should still be showing since second loader is active
            assert.strictEqual(adapter.hideCalls, 0);
        });

        it('handles loaders without messages', async () => {
            await loader.add(undefined, 0);
            await loader.add('With message', 0);

            assert.strictEqual(adapter.getLastMessage(), 'With message');
        });

        it('shows empty string when no loader has a message', async () => {
            await loader.add(undefined, 0);
            await loader.add(undefined, 0);

            assert.strictEqual(adapter.getLastMessage(), undefined);
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
            assert.strictEqual(adapter.showCalls[0].message, 'First'); // First message wins
        });

        it('does not increase delay with successive loaders', async () => {
            const promise1 = loader.add('First', 200);

            sinon.clock.tick(100);
            assert.strictEqual(adapter.showCalls.length, 0);

            const promise2 = loader.add('Second', 1000); // Should not reschedule

            sinon.clock.tick(100);
            await Promise.all([promise1, promise2]);

            assert.strictEqual(adapter.showCalls.length, 1);
            assert.strictEqual(adapter.showCalls[0].message, 'First');
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
                    assert.strictEqual(adapter.getLastMessage(), 'Working…');

                    operationComplete = true;
                    return 'result';
                },
                'Working…',
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
                    'Working…',
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
            const handle = await loader.add('Loading…', 0);

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
            assert.strictEqual(adapter.showCalls[0].message, 'First');
        });

        it('handles adapter errors gracefully in show', async () => {
            adapter.shouldThrow = true;

            try {
                await loader.add('Loading…', 0);
                // The error should be thrown from the adapter
                assert.fail('Expected error to be thrown');
            } catch (error) {
                assert.strictEqual((error as Error).message, 'Mock adapter error');
            }
        });

        describe('loader options functionality', () => {
            it('passes loader options to adapter', async () => {
                const options: TestOptions = { priority: 1, theme: 'dark' };
                await loader.add('Loading with options', 0, options);

                assert.strictEqual(adapter.showCalls.length, 1);
                assert.strictEqual(adapter.getLastMessage(), 'Loading with options');
                assert.deepStrictEqual(adapter.getLastOptions(), options);
            });

            it('uses options from most recent unresolved loader', async () => {
                const options1: TestOptions = { priority: 1, theme: 'light' };
                const options2: TestOptions = { priority: 2, theme: 'dark' };

                const handle1 = await loader.add('First', 0, options1);
                await loader.add('Second', 0, options2);

                // Should use options2 since it's the most recent
                assert.deepStrictEqual(adapter.getLastOptions(), options2);

                // Resolve the second loader
                loader.resolve(handle1);
                sinon.clock.tick(0);

                // Should still use options2 since second loader is still active
                assert.deepStrictEqual(adapter.getLastOptions(), options2);
            });

            it('handles loaders without options', async () => {
                await loader.add('No options', 0);

                assert.strictEqual(adapter.showCalls.length, 1);
                assert.strictEqual(adapter.getLastMessage(), 'No options');
                assert.strictEqual(adapter.getLastOptions(), undefined);
            });

            it('mixes loaders with and without options', async () => {
                const options: TestOptions = { priority: 3, theme: 'auto' };

                await loader.add('Without options', 0);
                await loader.add('With options', 0, options);

                // Should use options from the most recent loader
                assert.deepStrictEqual(adapter.getLastOptions(), options);
            });

            it('updates options when new loader is added', async () => {
                const options1: TestOptions = { priority: 1 };
                const options2: TestOptions = { priority: 2 };

                await loader.add('First', 0, options1);
                assert.deepStrictEqual(adapter.getLastOptions(), options1);

                await loader.add('Second', 0, options2);
                assert.deepStrictEqual(adapter.getLastOptions(), options2);
            });
        });

        describe('updateMessage functionality', () => {
            it('updates message for active loader', async () => {
                const handle = await loader.add('Initial message', 0);

                assert.strictEqual(adapter.getLastMessage(), 'Initial message');

                loader.updateMessage(handle, 'Updated message');
                await sinon.clock.tickAsync(0); // Allow async operations to complete

                // Should trigger a new show call with updated message
                assert.strictEqual(adapter.getLastMessage(), 'Updated message');
            });

            it('updates message to undefined', async () => {
                const handle = await loader.add('Initial message', 0);

                loader.updateMessage(handle, undefined);
                await sinon.clock.tickAsync(0); // Allow async operations to complete

                assert.strictEqual(adapter.getLastMessage(), undefined);
            });

            it('throws error for invalid handle', () => {
                assert.throws(() => {
                    loader.updateMessage(999, 'New message');
                }, /invalid handle 999/);
            });

            it('throws error for already resolved handle', async () => {
                const handle1 = await loader.add('Message 1', 0);
                const handle2 = await loader.add('Message 2', 0);
                const handle3 = await loader.add('Message 3', 0);

                // Resolve handle2 (middle one) - this marks it as resolved but doesn't remove it
                // because handle3 is still unresolved and comes after it
                loader.resolve(handle2);

                // Now try to update the resolved handle2
                assert.throws(() => {
                    loader.updateMessage(handle2, 'New message');
                }, /handle \d+ already resolved/);

                // Clean up
                loader.resolve(handle3);
                loader.resolve(handle1);
            });

            it('updates message when loader is scheduled to show', async () => {
                const promise = loader.add('Initial', 500);
                const handle = await promise;

                // Update message before show executes
                loader.updateMessage(handle, 'Updated before show');

                // Show should execute with updated message
                sinon.clock.tick(500);
                await sinon.clock.tickAsync(0); // Allow async operations to complete
                assert.strictEqual(adapter.getLastMessage(), 'Updated before show');
            });

            it('updates message for stacked loaders', async () => {
                const handle1 = await loader.add('First', 0);
                const handle2 = await loader.add('Second', 0);

                // Update first loader's message
                loader.updateMessage(handle1, 'Updated first');
                await sinon.clock.tickAsync(0); // Allow async operations to complete

                // Should show updated first message (earliest unresolved)
                assert.strictEqual(adapter.getLastMessage(), 'Updated first');

                // Update second loader's message
                loader.updateMessage(handle2, 'Updated second');
                await sinon.clock.tickAsync(0); // Allow async operations to complete

                // Should still show first message since it's earlier
                assert.strictEqual(adapter.getLastMessage(), 'Updated first');

                // Resolve first loader
                loader.resolve(handle1);
                await sinon.clock.tickAsync(0); // Allow async operations to complete

                // Now should show updated second message
                assert.strictEqual(adapter.getLastMessage(), 'Updated second');
            });

            it('handles rapid message updates', async () => {
                const handle = await loader.add('Initial', 0);

                loader.updateMessage(handle, 'Update 1');
                loader.updateMessage(handle, 'Update 2');
                loader.updateMessage(handle, 'Final update');

                await sinon.clock.tickAsync(0); // Allow async operations to complete

                assert.strictEqual(adapter.getLastMessage(), 'Final update');
            });

            it('does not trigger show when loader is scheduled', async () => {
                const promise = loader.add('Initial', 1000);
                const handle = await promise;

                const initialShowCalls = adapter.showCalls.length;

                // Update message while show is scheduled
                loader.updateMessage(handle, 'Updated');

                // Should not trigger additional show calls
                assert.strictEqual(adapter.showCalls.length, initialShowCalls);

                // When scheduled show executes, it should use updated message
                sinon.clock.tick(1000);
                await sinon.clock.tickAsync(0); // Allow async operations to complete
                assert.strictEqual(adapter.getLastMessage(), 'Updated');
            });
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
