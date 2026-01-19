import { AbstractAudioService } from '@common/service/AbstractAudioService';
import { AbstractEmulationService } from '@common/service/AbstractEmulationService';
import { Session } from '@embedded/model/Session';

export class EmbeddedAudioService extends AbstractAudioService {
    constructor(
        private session: Session,
        emulationService: AbstractEmulationService,
        pcmWorkletUrl?: string,
    ) {
        super(emulationService, pcmWorkletUrl);
    }

    public setVolume(volume: number): void {
        this.volume = Math.max(Math.min(volume, 1), 0);

        this.updateGain();
    }

    public getVolume(): number {
        return this.volume;
    }

    public setSession(session: Session): void {
        this.session = session;
        this.updateState();
    }

    protected runHidden(): boolean {
        return this.session.runInBackground;
    }

    private volume = 0.5;
}
