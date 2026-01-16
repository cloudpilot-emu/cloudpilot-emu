import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { cachedInstantiate, loadAndCompileModule } from '@common/helper/wasm';

import { Emulator, EmulatorImpl } from './Emulator';

export { DeviceId } from '@common/model/DeviceId';
export { Button } from './button';
export { DeviceOrientation } from '@common/model/DeviceOrientation';
export { Emulator } from './Emulator';
export { EmulationStatistics } from '@common/model/EmulationStatistics';
export { EmulatorEventTarget as EventTarget, EventHandler } from '@common/service/GenericEventHandlingService';
export { CloudpilotEvent as Event, Handler } from './Event';
export { SerialPort, ReceivePayload } from './SerialPort';

export const VERSION = process.env.VERSION;

/**
 * Options for loading the emulator.
 */
export interface LoadOptions {
    /**
     * URL for loading the cloudpilot WASM binary.
     */
    cloudpilotModuleUrl?: string;

    /**
     * URL for loading the uARM WASM binary (for OS5 emulator).
     */
    uarmModuleUrl?: string;

    /**
     * By default, the uARM WASM binary is loaded on demand when an OS5 session is
     * launched. This option causes the binary to be preloaded on initialization.
     */
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
 * Create a new instance of the emulator.
 *
 * @param options Optional: options for loading the emulator
 *
 * @returns Emulator instance
 */
export async function createEmulator(options: LoadOptions = {}): Promise<Emulator> {
    return createEmulatorFactory(options)();
}

/**
 * Create a factory function that creates new Emulator instances without redownloading
 * and recompiling the WASM module.
 *
 * @param options Optional: options for loading the emulator
 * @returns
 */
export function createEmulatorFactory(options: LoadOptions = {}): () => Promise<Emulator> {
    const instantiate = cachedInstantiate(options.cloudpilotModuleUrl ?? 'cloudpilot_web.wasm');
    const factory = creaeteUarmModuleFactory(options.preloadUarm ?? false, options.uarmModuleUrl ?? 'uarm_web.wasm');

    return () => Cloudpilot.create(instantiate).then((cloudpilot) => new EmulatorImpl(cloudpilot, factory));
}
