import { PalmButton } from '@common/bridge/Cloudpilot';
import { AbstractCanvasDisplayService } from '@common/service/AbstractCanvasDisplayService';
import { AbstractEmulationService } from '@common/service/AbstractEmulationService';

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

/**
 * DOM event handler callback.
 */
export type EventHandler<K extends keyof HTMLElementEventMap> = (ev: HTMLElementEventMap[K]) => void;

/**
 * A DOM event target.
 */
export interface EventTarget {
    addEventListener<K extends keyof HTMLElementEventMap>(
        type: K,
        handler: EventHandler<K>,
        options?: boolean | AddEventListenerOptions,
    ): void;
    removeEventListener<K extends keyof HTMLElementEventMap>(
        type: K,
        listener: EventHandler<K>,
        options?: boolean | EventListenerOptions,
    ): void;
}

export class GenericEventHandlingService {
    constructor(
        protected emulationService: AbstractEmulationService,
        protected canvasDisplayService: AbstractCanvasDisplayService,
    ) {}

    bind(pointerEventTarget: EventTarget, keyEventTarget: EventTarget = window): void {
        if (this.pointerEventTarget) {
            this.release();
        }

        this.pointerEventTarget = pointerEventTarget;
        this.keyEventTarget = keyEventTarget || window;

        this.addEventListener(pointerEventTarget, 'mousedown', this.handleMouseDown);
        this.addEventListener(window, 'mouseup', this.handeMouseUp);
        this.addEventListener(window, 'mousemove', this.handleMouseMove);

        this.addEventListener(pointerEventTarget, 'touchstart', this.handleTouchStart);
        this.addEventListener(pointerEventTarget, 'touchmove', this.handleTouchMove);
        this.addEventListener(pointerEventTarget, 'touchend', this.handleTouchEnd);
        this.addEventListener(pointerEventTarget, 'touchcancel', this.handleTouchEnd);

        this.addEventListener(this.keyEventTarget, 'keydown', this.handleKeyDown);
        this.addEventListener(this.keyEventTarget, 'keyup', this.handleKeyUp);
    }

    release(): void {
        if (!this.pointerEventTarget) return;

        this.removeEventListener(this.pointerEventTarget, 'mousedown', this.handleMouseDown);
        this.removeEventListener(window, 'mouseup', this.handeMouseUp);
        this.removeEventListener(window, 'mousemove', this.handleMouseMove);

        this.removeEventListener(this.pointerEventTarget, 'touchstart', this.handleTouchStart);
        this.removeEventListener(this.pointerEventTarget, 'touchmove', this.handleTouchMove);
        this.removeEventListener(this.pointerEventTarget, 'touchend', this.handleTouchEnd);
        this.removeEventListener(this.pointerEventTarget, 'touchcancel', this.handleTouchEnd);

        this.removeEventListener(this.keyEventTarget, 'keydown', this.handleKeyDown);
        this.removeEventListener(this.keyEventTarget, 'keyup', this.handleKeyUp);

        for (const button of this.activeButtons) {
            this.handleButtonUp(button);
        }

        this.activeButtons.clear();
        this.interactionsTouch.clear();
        this.interactionMouse = undefined;

        this.pointerEventTarget = undefined;
    }

    isGameMode(): boolean {
        return this.gameMode;
    }

    setGameMode(enableGameMode: boolean): void {
        this.gameMode = enableGameMode;

        this.onToggleGameMode();
    }

    setGameModeHotkeyEnabled(enableGamemodeHotkey: boolean): void {
        this.enableGamemodeHotkey = enableGamemodeHotkey;
    }

    isGameModeHotkeyEnabled(): boolean {
        return this.enableGamemodeHotkey;
    }

    protected enableKeyboardEvents(): boolean {
        return true;
    }

    protected onToggleGameMode(): void {}

    protected addEventListener<K extends keyof HTMLElementEventMap, E extends EventTarget>(
        elt: E,
        type: K,
        handler: EventHandler<K>,
        caputure?: boolean,
    ) {
        elt.addEventListener(type, handler, caputure);
    }

    protected removeEventListener<K extends keyof HTMLElementEventMap, E extends EventTarget>(
        elt: E,
        type: K,
        handler: EventHandler<K>,
        caputure?: boolean,
    ) {
        elt.removeEventListener(type, handler, caputure);
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
        let handled = false;

        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const coords = this.canvasDisplayService.eventToPalmCoordinates(touch);
            if (!coords) continue;

            handled = true;

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

        if (handled && e.cancelable !== false) e.preventDefault();
    };

    private handleTouchMove = (e: TouchEvent): void => {
        let handled = false;

        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const interaction = this.interactionsTouch.get(touch.identifier);
            if (!interaction) continue;

            handled = true;

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

        if (handled && e.cancelable !== false) e.preventDefault();
    };

    private handleTouchEnd = (e: TouchEvent): void => {
        let handled = false;

        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const interaction = this.interactionsTouch.get(touch.identifier);
            if (interaction) handled = true;

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

        if (handled && e.cancelable !== false) e.preventDefault();
    };

    private handleKeyDown = (e: KeyboardEvent): void => {
        if (this.emulationService.isSuspended() || !this.enableKeyboardEvents()) return;

        if (this.isToggleGameMode(e)) {
            e.preventDefault();

            this.gameMode = !this.gameMode;
            this.onToggleGameMode();

            return;
        }

        const button = this.buttonFromEvent(e);
        if (button !== undefined) {
            this.handleButtonDown(button);
            e.preventDefault();

            return;
        }

        const keyCode = this.keyCodeFromEvent(e);
        if (keyCode !== undefined && !e.ctrlKey && !e.metaKey) {
            this.emulationService.handleKeyDown(keyCode);
            e.preventDefault();
        }
    };

    private handleKeyUp = (e: KeyboardEvent): void => {
        if (this.emulationService.isSuspended() || !this.enableKeyboardEvents()) return;

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
            void this.canvasDisplayService.drawButtons([]);
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
        void this.canvasDisplayService.drawButtons(Array.from(this.activeButtons.values()));
    }

    private handleButtonUp(button: PalmButton | undefined): void {
        if (button === undefined || !this.activeButtons.has(button)) return;

        this.activeButtons.delete(button);
        this.emulationService.handleButtonUp(button);
        void this.canvasDisplayService.drawButtons(Array.from(this.activeButtons.values()));
    }

    private isToggleGameMode(e: KeyboardEvent): boolean {
        return (
            this.isGameModeHotkeyEnabled() && ((e.key === 'Shift' && e.ctrlKey) || (e.key === 'Control' && e.shiftKey))
        );
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

    private pointerEventTarget: EventTarget | undefined;
    private keyEventTarget: EventTarget = window;

    private interactionMouse: Interaction | undefined;
    private interactionsTouch = new Map<number, Interaction>();
    private activeButtons = new Set<PalmButton>();
    private gameMode = false;
    private enableGamemodeHotkey = true;
}
