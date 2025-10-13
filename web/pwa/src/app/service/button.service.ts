import { Injectable } from '@angular/core';
import { PalmButton } from '@common/bridge/Cloudpilot';
import { Engine } from '@common/engine/Engine';

interface ButtonContext {
    button: PalmButton;
    timer: number;
}

const PRESS_DURATION = 250;

@Injectable({ providedIn: 'root' })
export class ButtonService {
    reset(engine: Engine): void {
        this.engine = engine;
        this.buttons.clear();

        [PalmButton.power, PalmButton.cradle].forEach((button) => engine.buttonUp(button));
    }

    engage(button: PalmButton): void {
        if (this.buttons.has(button)) {
            this.buttons.get(button)!.timer = 0;
        } else {
            this.buttons.set(button, {
                button,
                timer: 0,
            });

            this.engine?.buttonDown(button);
        }
    }

    tick(seconds: number): void {
        for (const [button, ctx] of this.buttons) {
            ctx.timer += seconds * 1000;

            if (ctx.timer > PRESS_DURATION) {
                this.engine?.buttonUp(button);

                this.buttons.delete(button);
            }
        }
    }

    private engine: Engine | undefined;
    private buttons = new Map<PalmButton, ButtonContext>();
}
