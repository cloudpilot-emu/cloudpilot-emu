import { ArrayType, BuiltinType } from '@angular/compiler';

import { ArgumentOutOfRangeError } from 'rxjs';
import { BrowserStack } from 'protractor/built/driverProviders';
import { Button } from 'protractor';
import { CanvasHelper } from './CanvasHelper';
import { EmulationService } from './../../../service/emulation.service';
import { PalmButton } from '../../../../../../src';

const enum Area {
    silkscreen,
    buttons,
}

interface InteractionSilkscreen {
    area: Area.silkscreen;
}

interface InteractionButton {
    area: Area.buttons;
    button: PalmButton;
}

type Interaction = InteractionSilkscreen | InteractionButton;

export class EventHandler {
    constructor(
        private canvas: HTMLCanvasElement,
        private emulationService: EmulationService,
        private canvasHelper: CanvasHelper
    ) {}

    bind(): void {
        this.canvas.addEventListener('mousedown', this.handleMouseDown);
        this.canvas.addEventListener('mouseup', this.handeMouseUp);
        this.canvas.addEventListener('mousemove', this.handleMouseMove);

        this.canvas.addEventListener('touchstart', this.handleTouchStart);
        this.canvas.addEventListener('touchmove', this.handleTouchMove);
        this.canvas.addEventListener('touchend', this.handleTouchEnd);
        this.canvas.addEventListener('touchcancel', this.handleTouchEnd);
    }

    release(): void {
        this.canvas.removeEventListener('mousedown', this.handleMouseDown);
        this.canvas.removeEventListener('mouseup', this.handeMouseUp);
        this.canvas.removeEventListener('mousemove', this.handleMouseMove);

        this.canvas.removeEventListener('touchstart', this.handleTouchStart);
        this.canvas.removeEventListener('touchmove', this.handleTouchMove);
        this.canvas.removeEventListener('touchend', this.handleTouchEnd);
        this.canvas.removeEventListener('touchcancel', this.handleTouchEnd);
    }

    handleMouseDown = (e: MouseEvent): void => {
        if (e.button !== 0) return;

        const coords = this.eventToPalmCoordinates(e);
        if (!coords) return;

        const area = this.determineArea(coords);

        if (area === Area.silkscreen) {
            this.interactionMouse = { area };
            this.emulationService.handlePointerMove(...coords);
        } else {
            const button = this.determineButton(coords);
            this.interactionMouse = { area, button };

            this.emulationService.handleButtonDown(button);

            this.canvasHelper.drawButtons(this.activeButtons());
        }
    };

    handleMouseMove = (e: MouseEvent): void => {
        // tslint:disable-next-line: no-bitwise
        if (!(e.buttons & 0x01) || this.interactionMouse?.area !== Area.silkscreen) return;

        const coords = this.eventToPalmCoordinates(e);
        if (!coords) return;

        this.emulationService.handlePointerMove(...coords);
    };

    handeMouseUp = (e: MouseEvent): void => {
        if (e.button !== 0) return;

        const interaction = this.interactionMouse;
        this.interactionMouse = undefined;

        switch (interaction?.area) {
            case Area.buttons:
                this.emulationService.handleButtonUp(interaction.button);

                this.canvasHelper.drawButtons(this.activeButtons());

                break;

            case Area.silkscreen:
                this.emulationService.handlePointerUp();

                break;
        }
    };

    handleTouchStart = (e: TouchEvent): void => {
        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const coords = this.eventToPalmCoordinates(touch);
            if (!coords) continue;

            const area = this.determineArea(coords);
            if (area === Area.silkscreen) {
                this.interactionsTouch.set(touch.identifier, { area });
                this.emulationService.handlePointerMove(...coords);
            } else {
                const button = this.determineButton(coords);
                this.interactionsTouch.set(touch.identifier, { area, button });

                this.emulationService.handleButtonDown(button);

                this.canvasHelper.drawButtons(this.activeButtons());
            }
        }

        e.preventDefault();
    };

    handleTouchMove = (e: TouchEvent): void => {
        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            if (this.interactionsTouch.get(touch.identifier)?.area === Area.silkscreen) {
                const coords = this.eventToPalmCoordinates(touch);
                if (!coords) continue;

                this.emulationService.handlePointerMove(...coords);
            }
        }

        e.preventDefault();
    };

    handleTouchEnd = (e: TouchEvent): void => {
        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const interaction = this.interactionsTouch.get(touch.identifier);
            this.interactionsTouch.delete(touch.identifier);

            switch (interaction?.area) {
                case Area.buttons:
                    this.emulationService.handleButtonUp(interaction.button);

                    this.canvasHelper.drawButtons(this.activeButtons());

                    break;

                case Area.silkscreen:
                    this.emulationService.handlePointerUp();

                    break;
            }
        }

        e.preventDefault();
    };

    private eventToPalmCoordinates(e: MouseEvent | Touch): [number, number] | undefined {
        const bb = this.canvas.getBoundingClientRect();

        let contentX: number;
        let contentY: number;
        let contentWidth: number;
        let contentHeight: number;

        // CSS object-fit keeps the aspect ratio of the canvas content, but the canvas itself
        // looses aspect and fills the container -> manually calculate the metrics for the content
        if (bb.width / bb.height > 160 / 250) {
            contentHeight = bb.height;
            contentWidth = (160 / 250) * bb.height;
            contentY = bb.top;
            contentX = bb.left + (bb.width - contentWidth) / 2;
        } else {
            contentWidth = bb.width;
            contentHeight = (250 / 160) * bb.width;
            contentX = bb.left;
            contentY = bb.top + (bb.height - contentHeight) / 2;
        }

        const x = Math.floor(((e.clientX - contentX) / contentWidth) * 160);
        const y = Math.floor(((e.clientY - contentY) / contentHeight) * 250);

        if (x < 0 || x >= 160 || y < 0 || y >= 250) return undefined;

        return [x, y];
    }

    private determineArea([, y]: [number, number]): Area {
        return y >= 220 ? Area.buttons : Area.silkscreen;
    }

    private determineButton([x, y]: [number, number]): PalmButton {
        if (x >= 130) return PalmButton.notes;
        if (x >= 100) return PalmButton.todo;
        if (x >= 60) return y >= 235 ? PalmButton.down : PalmButton.up;
        if (x >= 30) return PalmButton.phone;

        return PalmButton.cal;
    }

    private activeButtons(): Array<PalmButton> {
        const buttons = Array.from(this.interactionsTouch.values())
            .filter((i) => i.area === Area.buttons)
            .map((i) => (i as InteractionButton).button);

        if (this.interactionMouse?.area === Area.buttons) buttons.push(this.interactionMouse.button);

        return buttons;
    }

    private interactionMouse: Interaction | undefined;
    private interactionsTouch = new Map<number, Interaction>();
}
