import { AbstractAudioService } from '@common/service/AbstractAudioService';

export class EmbeddedAudioService extends AbstractAudioService {
    public setVolume(volume: number): void {
        this.volume = Math.max(Math.min(volume, 1), 0);

        this.updateGain();
    }

    public getVolume(): number {
        return this.volume;
    }
    protected runHidden(): boolean {
        return false;
    }

    private volume = 0.5;
}
