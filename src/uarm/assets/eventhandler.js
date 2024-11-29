export class EventHandler {
    constructor(emulator, displayService) {
        this.emulator = emulator;
        this.displayService = displayService;

        this.keyEventTarget = window;
        this.interactionsTouch = new Map();
        this.activeButtons = new Set();

        this.handeMouseUp = this.handeMouseUp.bind(this);
        this.handleMouseDown = this.handleMouseDown.bind(this);
        this.handleMouseMove = this.handleMouseMove.bind(this);

        this.handleTouchStart = this.handleTouchStart.bind(this);
        this.handleTouchMove = this.handleTouchMove.bind(this);
        this.handleTouchEnd = this.handleTouchEnd.bind(this);

        this.handleKeyDown = this.handleKeyDown.bind(this);
        this.handleKeyUp = this.handleKeyUp.bind(this);
    }

    bind(pointerEventTarget, keyEventTarget = window) {
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

    release() {
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

    addEventListener(elt, type, handler, caputure) {
        elt.addEventListener(type, handler, caputure);
    }

    removeEventListener(elt, type, handler, caputure) {
        elt.removeEventListener(type, handler, caputure);
    }

    handleMouseDown(e) {
        if (e.button !== 0) return;

        const coords = this.displayService.eventToPalmCoordinates(e);
        if (!coords) return;

        const area = this.determineArea(coords);

        if (area === 'buttons') {
            const button = this.displayService.determineButton(coords);
            this.interactionMouse = { area, buttonPrimary: button };

            this.handleButtonDown(button);
        } else {
            this.interactionMouse = { area };
            this.emulator.penDown(...coords);
        }
    }

    handleMouseMove(e) {
        if (!(e.buttons & 0x01) || !this.interactionMouse || this.interactionMouse?.area === 'buttons') {
            return;
        }

        const coords = this.displayService.eventToPalmCoordinates(e, true);
        if (!coords) return;

        this.emulator.penDown(...coords);
    }

    handeMouseUp(e) {
        if (e.button !== 0) return;

        const interaction = this.interactionMouse;
        this.interactionMouse = undefined;

        switch (interaction?.area) {
            case 'buttons':
                this.handleButtonUp(interaction.buttonPrimary);

                break;

            case 'screen':
            case 'silkscreen':
                this.emulator.penUp();

                break;
        }
    }

    handleTouchStart(e) {
        let handled = false;

        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const coords = this.displayService.eventToPalmCoordinates(touch);
            if (!coords) continue;

            handled = true;

            const area = this.determineArea(coords);
            if (area === 'buttons') {
                const button = this.displayService.determineButton(coords);
                this.interactionsTouch.set(touch.identifier, { area, buttonPrimary: button });

                this.handleButtonDown(button);
            } else {
                this.interactionsTouch.set(touch.identifier, { area });
                this.emulator.penDown(...coords);
            }
        }

        if (handled && e.cancelable !== false) e.preventDefault();
    }

    handleTouchMove(e) {
        let handled = false;

        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const interaction = this.interactionsTouch.get(touch.identifier);
            if (!interaction) continue;

            handled = true;

            const area = interaction.area;
            switch (area) {
                case 'buttons': {
                    const coords = this.displayService.eventToPalmCoordinates(touch);

                    if (!coords || this.determineArea(coords) !== 'buttons') {
                        this.handleButtonUp(interaction.buttonSecondary);
                        interaction.buttonSecondary = undefined;

                        break;
                    }

                    const button = this.displayService.determineButton(coords);

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

                case 'screen':
                case 'silkscreen': {
                    const coords = this.displayService.eventToPalmCoordinates(touch, true);
                    if (!coords) continue;

                    this.emulator.penDown(...coords);

                    break;
                }

                default:
                    continue;
            }
        }

        if (handled && e.cancelable !== false) e.preventDefault();
    }

    handleTouchEnd(e) {
        let handled = false;

        for (let i = 0; i < e.changedTouches.length; i++) {
            const touch = e.changedTouches.item(i);
            if (!touch) continue;

            const interaction = this.interactionsTouch.get(touch.identifier);
            if (interaction) handled = true;

            this.interactionsTouch.delete(touch.identifier);

            switch (interaction?.area) {
                case 'buttons':
                    this.handleButtonUp(interaction.buttonPrimary);
                    this.handleButtonUp(interaction.buttonSecondary);

                    break;

                case 'screen':
                case 'silkscreen':
                    this.emulator.penUp();

                    break;
            }
        }

        if (handled && e.cancelable !== false) e.preventDefault();
    }

    handleKeyDown(e) {
        const button = this.buttonFromEvent(e);
        if (button !== undefined) {
            this.handleButtonDown(button);
            e.preventDefault();

            return;
        }
    }

    handleKeyUp(e) {
        const button = this.buttonFromEvent(e);

        if (button !== undefined) {
            this.handleButtonUp(button);
            e.preventDefault();

            return;
        }
    }

    determineArea(coords) {
        if (this.displayService.isButtons(coords)) {
            return 'buttons';
        }

        if (this.displayService.isSilkscreen(coords)) {
            return 'silkscreen';
        }

        return 'screen';
    }

    handleButtonDown(button) {
        if (this.activeButtons.has(button)) return;

        this.activeButtons.add(button);
        this.emulator.buttonDown(button);
        void this.displayService.drawButtons(Array.from(this.activeButtons.values()));
    }

    handleButtonUp(button) {
        if (button === undefined || !this.activeButtons.has(button)) return;

        this.activeButtons.delete(button);
        this.emulator.buttonUp(button);
        void this.displayService.drawButtons(Array.from(this.activeButtons.values()));
    }

    buttonFromEvent(e) {
        switch (e.key) {
            case 'PageUp':
                return 'up';

            case 'PageDown':
                return 'down';
        }

        switch (e.key) {
            case 'w':
            case 'i':
            case 'ArrowUp':
                return 'up';

            case 's':
            case 'k':
            case 'ArrowDown':
                return 'down';

            case 'a':
            case 'j':
            case 'y':
            case 'z':
            case 'ArrowLeft':
                return 'cal';

            case 'q':
            case 'u':
            case 'x':
                return 'phone';

            case 'e':
            case 'o':
            case 'c':
                return 'todo';

            case 'd':
            case 'l':
            case 'v':
            case 'ArrowRight':
                return 'notes';
        }

        return undefined;
    }
}
