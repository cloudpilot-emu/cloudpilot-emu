import { Uarm } from '@common/bridge/Uarm';
import { EngineSettings } from '@common/engine/EngineSettings';

export class Emulator {
    constructor(
        private uarm: Uarm,
        private settings: EngineSettings,
    ) {}

    openSession(
        rom: Uint8Array,
        nand?: Uint8Array,
        memory?: Uint8Array,
        state?: Uint8Array,
        card?: [Uint8Array, string],
    ): boolean {
        // This is a slight hack, but I see no cleaner way to do this. If the session is new, then
        // memory will be undefined, and the logic in the app will default to
        // RomInfo5::GetRecommendedRamSize . The default in Uarm::launch is the same, which is
        // why this works.
        const ramSize = memory ? (memory.length >= 32 << 20 ? 32 << 20 : 16 << 20) : undefined;

        if (ramSize !== undefined) this.uarm.setRamSize(ramSize);
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
