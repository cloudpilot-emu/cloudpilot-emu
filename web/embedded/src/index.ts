import { Emulator, EmulatorImpl } from './Emulator';

import { Cloudpilot } from '@common/Cloudpilot';

export { DeviceId } from '@common/model/DeviceId';
export { Button } from './button';
export { DeviceOrientation } from '@common/model/DeviceOrientation';
export { Emulator } from './Emulator';
export { EmulationStatistics } from '@common/model/EmulationStatistics';
export { EventSource, EventHandler } from '@common/service/GenericEventHandlingService';
export { Event, Handler } from './Event';

export const VERSION = 'dev';

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
