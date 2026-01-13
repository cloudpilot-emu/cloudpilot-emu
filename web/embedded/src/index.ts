import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { cachedInstantiate, loadAndCompileModule } from '@common/helper/wasm';

import { Emulator, EmulatorImpl } from './Emulator';

export { DeviceId } from '@common/model/DeviceId';
export { Button } from './button';
export { DeviceOrientation } from '@common/model/DeviceOrientation';
export { Emulator } from './Emulator';
export { EmulationStatistics } from '@common/model/EmulationStatistics';
export { EventTarget, EventHandler } from '@common/service/GenericEventHandlingService';
export { Event, Handler } from './Event';
export { SerialPort, ReceivePayload } from './SerialPort';

export const VERSION = process.env.VERSION;

export interface Options {
    cloudpilotModuleUrl?: string;
    uarmModuleUrl?: string;
    preloadUarm?: boolean;
}

const creaeteUarmModuleFactory = (preload: boolean, url: string): (() => Promise<WebAssembly.Module>) => {
    let module: Promise<WebAssembly.Module> | undefined;

    const factory = () => {
        if (!module) module = loadAndCompileModule(url);
        return module;
    };

    if (preload) void factory();

    return factory;
};

/**
 * Create a factory function that creates new Emulator instances without redownloading
 * and recompiling the WASM module.
 *
 * @param wasmModuleUrl Optional: URL for loading the web assembly module
 * @returns
 */
export function createEmulatorFactory(options: Options = {}): () => Promise<Emulator> {
    const instantiate = cachedInstantiate(options.cloudpilotModuleUrl ?? 'cloudpilot_web.wasm');
    const factory = creaeteUarmModuleFactory(options.preloadUarm ?? false, options.uarmModuleUrl ?? 'uarm_web.wasm');

    return () => Cloudpilot.create(instantiate).then((cloudpilot) => new EmulatorImpl(cloudpilot, factory));
}

/**
 * Create a new instance of the emulator.
 *
 * @param wasmModuleUrl Optional: URL for loading the web assembly module
 *
 * @returns Emulator instance
 */
export async function createEmulator(options: Options = {}): Promise<Emulator> {
    return createEmulatorFactory(options)();
}
