import { AbstractEmulationService } from '@common/service/AbstractEmulationService';
import { EmbeddedCanvasDisplayService } from './EmbeddedCanvasDisplayService';
import { GenericEventHandlingService } from '@common/service/GenericEventHandlingService';

export class EmbeddedEventHandlingServie extends GenericEventHandlingService {
    constructor(emulationService: AbstractEmulationService, canvasDisplayService: EmbeddedCanvasDisplayService) {
        super(emulationService, canvasDisplayService);

        this.canvasDisplayService = canvasDisplayService;
    }

    protected override onToggleGameMode(): void {
        this.canvasDisplayService.toggleGameModeIndicator(this.isGameMode());
    }

    protected override canvasDisplayService: EmbeddedCanvasDisplayService;
}
