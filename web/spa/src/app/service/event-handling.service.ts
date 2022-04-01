import { Injectable, NgZone } from '@angular/core';

import { CanvasDisplayService } from './canvas-display.service';
import { EmulationService } from './emulation.service';
import { ModalWatcherService } from './modal-watcher.service';
import { PalmButton } from '../helper/Cloudpilot';

const enum Area {
    screen,
    silkscreen,
    buttons,
}

interface InteractionScreen {
    area: Area.screen | Area.silkscreen;
}

interface InteractionButton {
    area: Area.buttons;
    buttonPrimary: PalmButton;
    buttonSecondary?: PalmButton;
}

type Interaction = InteractionScreen | InteractionButton;

@Injectable({
    providedIn: 'root',
})
export class EventHandlingService {
    constructor(
        private emulationService: EmulationService,
        private canvasDisplayService: CanvasDisplayService,
        private modalWatcherService: ModalWatcherService,
        private ngZone: NgZone
    ) {}

    bind(canvas: HTMLCanvasElement): void {
        if (this.canvas) {
            this.release();
        }

        this.canvas = canvas;

        this.ngZone.runOutsideAngular(() => {
            canvas.addEventListener('mousedown', this.handleMouseDown);
            window.addEventListener('mouseup', this.handeMouseUp);
            window.addEventListener('mousemove', this.handleMouseMove);

            canvas.addEventListener('touchstart', this.handleTouchStart);
            canvas.addEventListener('touchmove', this.handleTouchMove);
            canvas.addEventListener('touchend', this.handleTouchEnd);
            canvas.addEventListener('touchcancel', this.handleTouchEnd);

            window.addEventListener('keydown', this.handleKeyDown);
            window.addEventListener('keyup', this.handleKeyUp);
        });
    }

    release(): void {
        if (!this.canvas) return;

        this.canvas.removeEventListener('mousedown', this.handleMouseDown);
        window.removeEventListener('mouseup', this.handeMouseUp);
        window.removeEventListener('mousemove', this.handleMouseMove);

        this.canvas.removeEventListener('touchstart', this.handleTouchStart);
        this.canvas.removeEventListener('touchmove', this.handleTouchMove);
        this.canvas.removeEventListener('touchend', this.handleTouchEnd);
        this.canvas.removeEventListener('touchcancel', this.handleTouchEnd);

        window.removeEventListener('keydown', this.handleKeyDown);
        window.removeEventListener('keyup', this.handleKeyUp);

        for (const button of this.activeButtons) {
            this.handleButtonUp(button);
        }

        this.activeButtons.clear();
        this.interactionsTouch.clear();
        this.interactionMouse = undefined;

        this.canvas = undefined;
    }

    isGameMode(): boolean {
        return this.gameMode;
    }

    private handleMouseDown = (e: MouseEvent): void => {
        if (e.button !== 0) return;

        const coords = this.canvasDisplayService.eventToPalmCoordinates(e);
        if (!coords) return;

        const area = this.determineArea(coords);

        if (area === Area.buttons) {
            const button = this.canvasDisplayService.determineButton(coords);
            this.interactionMouse = { area, buttonPrimary: button };

            this.handleButtonDown(button);
        } else {
            this.interactionMouse = { area };
            this.emulationService.handlePointerMove(...coords, area === Area.silkscreen);
        }
    };

    private handleMouseMove = (e: MouseEvent): void => {
        // tslint:disable-next-line: no-bitwise
        if (!(e.buttons & 0x01) || !this.interactionMouse || this.interactionMouse?.area === Area.buttons) {
            return;
        }

        const coords = this.canvasDisplayService.eventToPalmCoordinates(e, true);
        if (!coords) return;

        this.emulationService.handlePointerMove(...coords, this.interactionMouse?.area === Area.silkscreen);
    };

    private handeMouseUp = (e: MouseEvent): void => {
        if (e.button !== 0) return;

        const interaction = this.interactionMouse;
        this.interactionMouse = undefined;

        switch (interaction?.area) {
            case Area.buttons:
                this.handleButtonUp(interaction.buttonPrimary);

                break;

            case Area.screen:
            case Area.silkscreen:
                this.emulationService.handlePointerUp();

                break;
        }
    };

    private handleTouchStart = (e: TouchEvent): void => {
        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const coords = this.canvasDisplayService.eventToPalmCoordinates(touch);
            if (!coords) continue;

            const area = this.determineArea(coords);
            if (area === Area.buttons) {
                const button = this.canvasDisplayService.determineButton(coords);
                this.interactionsTouch.set(touch.identifier, { area, buttonPrimary: button });

                this.handleButtonDown(button);
            } else {
                this.interactionsTouch.set(touch.identifier, { area });
                this.emulationService.handlePointerMove(...coords, area === Area.silkscreen);
            }
        }

        if (e.cancelable !== false) e.preventDefault();
    };

    private handleTouchMove = (e: TouchEvent): void => {
        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const interaction = this.interactionsTouch.get(touch.identifier);
            if (!interaction) continue;

            const area = interaction.area;
            switch (area) {
                case Area.buttons: {
                    const coords = this.canvasDisplayService.eventToPalmCoordinates(touch);

                    if (!coords || this.determineArea(coords) !== Area.buttons) {
                        this.handleButtonUp(interaction.buttonSecondary);
                        interaction.buttonSecondary = undefined;

                        break;
                    }

                    const button = this.canvasDisplayService.determineButton(coords);

                    if (button === interaction.buttonPrimary) {
                        this.handleButtonUp(interaction.buttonSecondary);
                        interaction.buttonSecondary = undefined;
                    } else if (button !== interaction.buttonSecondary) {
                        this.handleButtonUp(interaction.buttonSecondary);
                        interaction.buttonSecondary = button;

                        this.handleButtonDown(button);
                    }

                    break;
                }

                case Area.screen:
                case Area.silkscreen: {
                    const coords = this.canvasDisplayService.eventToPalmCoordinates(touch, true);
                    if (!coords) continue;

                    this.emulationService.handlePointerMove(...coords, area === Area.silkscreen);

                    break;
                }

                default:
                    continue;
            }
        }

        if (e.cancelable !== false) e.preventDefault();
    };

    private handleTouchEnd = (e: TouchEvent): void => {
        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const interaction = this.interactionsTouch.get(touch.identifier);
            this.interactionsTouch.delete(touch.identifier);

            switch (interaction?.area) {
                case Area.buttons:
                    this.handleButtonUp(interaction.buttonPrimary);
                    this.handleButtonUp(interaction.buttonSecondary);

                    break;

                case Area.screen:
                case Area.silkscreen:
                    this.emulationService.handlePointerUp();

                    break;
            }
        }

        if (e.cancelable !== false) e.preventDefault();
    };

    private handleKeyDown = (e: KeyboardEvent): void => {
        if (this.emulationService.isSuspended() || this.modalWatcherService.isModalActive()) return;

        if (this.isToggleGameMode(e)) {
            e.preventDefault();

            this.ngZone.run(() => (this.gameMode = !this.gameMode));

            return;
        }

        const button = this.buttonFromEvent(e);
        if (button !== undefined) {
            this.handleButtonDown(button);
            e.preventDefault();

            return;
        }

        const keyCode = this.keyCodeFromEvent(e);
        if (keyCode !== undefined) {
            this.emulationService.handleKeyDown(keyCode);
            e.preventDefault();
        }
    };

    private handleKeyUp = (e: KeyboardEvent): void => {
        if (this.emulationService.isSuspended() || this.modalWatcherService.isModalActive()) return;

        const button = this.buttonFromEvent(e);
        if (button !== undefined) {
            this.handleButtonUp(button);
            e.preventDefault();

            return;
        }

        if (e.key === 'Control') {
            for (const btn of this.activeButtons) {
                this.emulationService.handleButtonUp(btn);
            }

            this.activeButtons.clear();
            this.canvasDisplayService.drawButtons([]);
        }
    };

    private determineArea(coords: [number, number]): Area {
        if (this.canvasDisplayService.isButtons(coords)) {
            return Area.buttons;
        }

        if (this.canvasDisplayService.isSilkscreen(coords)) {
            return Area.silkscreen;
        }

        return Area.screen;
    }

    private handleButtonDown(button: PalmButton): void {
        if (this.activeButtons.has(button)) return;

        this.activeButtons.add(button);
        this.emulationService.handleButtonDown(button);
        this.canvasDisplayService.drawButtons(Array.from(this.activeButtons.values()));
    }

    private handleButtonUp(button: PalmButton | undefined): void {
        if (button === undefined || !this.activeButtons.has(button)) return;

        this.activeButtons.delete(button);
        this.emulationService.handleButtonUp(button);
        this.canvasDisplayService.drawButtons(Array.from(this.activeButtons.values()));
    }

    private isToggleGameMode(e: KeyboardEvent): boolean {
        return (e.key === 'Shift' && e.ctrlKey) || (e.key === 'Control' && e.shiftKey);
    }

    private isGameModeActive(e: KeyboardEvent): boolean {
        return (!this.gameMode && e.ctrlKey) || (this.gameMode && !e.ctrlKey);
    }

    private buttonFromEvent(e: KeyboardEvent): PalmButton | undefined {
        switch (e.key) {
            case 'PageUp':
                return PalmButton.up;

            case 'PageDown':
                return PalmButton.down;
        }

        if (!this.isGameModeActive(e)) return;

        switch (e.key) {
            case 'w':
            case 'i':
            case 'ArrowUp':
                return PalmButton.up;

            case 's':
            case 'k':
            case 'ArrowDown':
                return PalmButton.down;

            case 'a':
            case 'j':
            case 'y':
            case 'z':
            case 'ArrowLeft':
                return PalmButton.cal;

            case 'q':
            case 'u':
            case 'x':
                return PalmButton.phone;

            case 'e':
            case 'o':
            case 'c':
                return PalmButton.todo;

            case 'd':
            case 'l':
            case 'v':
            case 'ArrowRight':
                return PalmButton.notes;
        }

        return undefined;
    }

    private keyCodeFromEvent(e: KeyboardEvent): number | undefined {
        if (this.isGameModeActive(e)) return;

        if (e.key.length === 1 && e.key.charCodeAt(0) <= 255) return e.key.charCodeAt(0);

        switch (e.key) {
            case 'ArrowLeft':
                return 0x1c;

            case 'ArrowRight':
                return 0x1d;

            case 'ArrowUp':
                return 0x1e;

            case 'ArrowDown':
                return 0x1f;

            case 'Backspace':
                return 0x08;

            case 'Tab':
                return 0x09;

            case 'Enter':
                return 0x0a;

            default:
                return undefined;
        }
    }

    private canvas: HTMLCanvasElement | undefined;
    private interactionMouse: Interaction | undefined;
    private interactionsTouch = new Map<number, Interaction>();
    private activeButtons = new Set<PalmButton>();
    private gameMode = false;
}
