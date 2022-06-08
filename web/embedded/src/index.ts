import { Emulator, EmulatorInterface } from './Emulator';

import { Cloudpilot } from '@common/Cloudpilot';

export const VERSION = 'dev';

export async function createEmulator(): Promise<EmulatorInterface> {
    return new Emulator(await Cloudpilot.create());
}
