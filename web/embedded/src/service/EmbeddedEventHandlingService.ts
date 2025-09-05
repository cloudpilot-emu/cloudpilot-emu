import { AbstractEmulationService } from '@common/service/AbstractEmulationService';
import { GenericEventHandlingService } from '@common/service/GenericEventHandlingService';
import { Event } from 'microevent.ts';

import { EmbeddedCanvasDisplayService } from './EmbeddedCanvasDisplayService';

export class EmbeddedEventHandlingServie extends GenericEventHandlingService {
    constructor(emulationService: AbstractEmulationService, canvasDisplayService: EmbeddedCanvasDisplayService) {
        super(emulationService, canvasDisplayService);

        this.canvasDisplayService = canvasDisplayService;
    }

    protected override onToggleGameMode(): void {
        this.canvasDisplayService.toggleGameModeIndicator(this.isGameMode());
        this.gameModeChangeEvent.dispatch(this.isGameMode());
    }

    gameModeChangeEvent = new Event<boolean>();

    protected override canvasDisplayService: EmbeddedCanvasDisplayService;
}
