import { Emulator, EmulatorImpl } from './Emulator';

import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { cachedInstantiate } from '@common/helper/wasm';

export { DeviceId } from '@common/model/DeviceId';
export { Button } from './button';
export { DeviceOrientation } from '@common/model/DeviceOrientation';
export { Emulator } from './Emulator';
export { EmulationStatistics } from '@common/model/EmulationStatistics';
export { EventTarget, EventHandler } from '@common/service/GenericEventHandlingService';
export { Event, Handler } from './Event';
export { SerialPort, ReceivePayload } from './SerialPort';

export const VERSION = process.env.VERSION;

/**
 * Create a new instance of the emulator.
 *
 * @param wasmModuleUrl Optional: URL for loading the web assembly module
 *
 * @returns Emulator instance
 */
export async function createEmulator(wasmModuleUrl?: string): Promise<Emulator> {
    return new EmulatorImpl(await Cloudpilot.create(wasmModuleUrl));
}

/**
 * Create a factory function that creates new Emulator instances without redownloading
 * and recompiling the WASM module.
 *
 * @param wasmModuleUrl Optional: URL for loading the web assembly module
 * @returns
 */
export function createEmulatorFactory(wasmModuleUrl?: string): () => Promise<Emulator> {
    const instantiate = cachedInstantiate(wasmModuleUrl ?? 'cloudpilot_web.wasm');

    return () => Cloudpilot.create(instantiate).then((cloudpilot) => new EmulatorImpl(cloudpilot));
}
