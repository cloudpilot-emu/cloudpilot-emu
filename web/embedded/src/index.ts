import { Cloudpilot, PalmButton } from '@common/Cloudpilot';
import { Emulator, EmulatorInterface } from './Emulator';

export { DeviceId } from '@common/model/DeviceId';
export { Button } from './button';
export { DeviceOrientation } from '@common/model/DeviceOrientation';

export const VERSION = 'dev';

export async function createEmulator(wasmModuleUrl?: string): Promise<EmulatorInterface> {
    return new Emulator(await Cloudpilot.create(wasmModuleUrl));
}
