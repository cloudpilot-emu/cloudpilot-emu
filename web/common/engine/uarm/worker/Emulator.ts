import { Uarm } from '@common/bridge/Uarm';
import { EngineSettings } from '@common/engine/EngineSettings';

export class Emulator {
    constructor(
        private uarm: Uarm,
        private settings: EngineSettings,
    ) {
        console.log(uarm, settings);
    }

    updateSettings(settings: EngineSettings): void {
        this.settings = settings;
    }
}
