import Cloudpilot from '../Cloudpilot';

class EmulationService {
    async initialize(rom: Uint8Array, deviceType: string): Promise<void> {
        const cp = await this.cloudpilot;

        const romInfo = cp.getRomInfo(rom);

        console.log(romInfo);

        if (!cp.initializeSession(rom, deviceType)) {
            throw new Error('unable to initialize session');
        }
    }

    private cloudpilot = Cloudpilot.create();
}

export default EmulationService;
