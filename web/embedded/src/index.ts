import { Cloudpilot } from '@common/Cloudpilot';

interface Emulator {}

export const VERSION = 'dev';

export async function createFromSession(sessionOrRom: Uint8Array): Promise<Emulator> {
    const cloudpilotInstance = await Cloudpilot.create();

    const session = cloudpilotInstance.deserializeSessionImage<unknown>(sessionOrRom);
    if (!session) {
        throw new Error('not a session');
    }

    return {};
}
