import { Cloudpilot, PalmButton } from '../helper/Cloudpilot';

import { Injectable } from '@angular/core';

interface ButtonContext {
    button: PalmButton;
    timer: number;
}

const PRESS_DURATION = 250;

@Injectable({ providedIn: 'root' })
export class ButtonService {
    reset(cloudpilot: Cloudpilot): void {
        this.cloudpilot = cloudpilot;
        this.buttons.clear();

        [PalmButton.power, PalmButton.cradle].forEach((button) => cloudpilot.queueButtonUp(button));
    }

    engage(button: PalmButton): void {
        if (!this.cloudpilot) return;

        if (this.buttons.has(button)) {
            this.buttons.get(button)!.timer = 0;
        } else {
            this.buttons.set(button, {
                button,
                timer: 0,
            });

            this.cloudpilot?.queueButtonDown(button);
        }
    }

    tick(seconds: number): void {
        if (!this.cloudpilot) return;

        for (const [button, ctx] of this.buttons) {
            ctx.timer += seconds * 1000;

            if (ctx.timer > PRESS_DURATION) {
                this.cloudpilot.queueButtonUp(button);

                this.buttons.delete(button);
            }
        }
    }

    private cloudpilot: Cloudpilot | undefined;
    private buttons = new Map<PalmButton, ButtonContext>();
}
