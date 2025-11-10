import { Uarm } from '@common/bridge/Uarm';
import { EngineSettings } from '@common/engine/EngineSettings';
import { ramSize as determineRamSize } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';

export class Emulator {
    constructor(
        private uarm: Uarm,
        private settings: EngineSettings,
    ) {}

    openSession(
        rom: Uint8Array,
        device: DeviceId,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: [Uint8Array, string],
    ): boolean {
        const ramSize =
            (memory ? (memory.length >= 32 << 20 ? 32 << 20 : 16 << 20) : undefined) ?? determineRamSize(device);
        if (ramSize === undefined) throw new Error(`invalid device id ${device}`);

        this.uarm.setRamSize(ramSize);
        if (nand) this.uarm.setNand(nand);
        if (memory) this.uarm.setMemory(memory);
        if (state) this.uarm.setSavestate(state);

        if (card) {
            const [cardData, key] = card;
            this.uarm.setSd(cardData, key);
        }

        return this.uarm.launch(rom);
    }

    updateSettings(settings: EngineSettings): void {
        this.settings = settings;
    }
}
