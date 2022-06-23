import { Cloudpilot, PalmButton } from '@common/Cloudpilot';
import { Emulator, EmulatorInterface } from './Emulator';

export { DeviceId } from '@common/model/DeviceId';
export { Button } from './button';

export const VERSION = 'dev';

export async function createEmulator(): Promise<EmulatorInterface> {
    return new Emulator(await Cloudpilot.create());
}
