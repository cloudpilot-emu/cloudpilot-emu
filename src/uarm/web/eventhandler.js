export class EventHandler {
    constructor(emulator, canvas) {
        this.canvas = canvas;

        const palmCoords = (e) => {
            const bb = this.canvas.getBoundingClientRect();
            const x = (e.clientX - bb.x) >>> 1;
            const y = (e.clientY - bb.y) >>> 1;

            return x < 0 || x >= 320 || y < 0 || y >= 440 ? undefined : { x, y };
        };

        this.onMouse = (e) => {
            if ((e.buttons & 1) === 0) return;

            const coords = palmCoords(e);
            if (!coords) return;

            e.stopPropagation();
            e.preventDefault();
            setImmediate(() => emulator.penDown(coords.x, coords.y));
        };

        this.onMouseUp = (e) => {
            if (e.button !== 0) return;

            setImmediate(() => emulator.penUp());
        };

        this.onTouch = (e) => {
            for (let i = 0; i < e.changedTouches.length; i++) {
                const touch = e.changedTouches[i];
                if (!touch) continue;

                const coords = palmCoords(touch);
                if (!coords) continue;

                e.stopPropagation();
                e.preventDefault();
                setImmediate(() => emulator.penDown(coords.x, coords.y));

                return;
            }
        };

        this.onTouchEnd = (e) => {
            setImmediate(() => emulator.penUp());
        };
    }

    start() {
        this.canvas.addEventListener('mousedown', this.onMouse);
        this.canvas.addEventListener('mousemove', this.onMouse);
        this.canvas.addEventListener('touchstart', this.onTouch);
        this.canvas.addEventListener('touchmove', this.onTouch);
        window.addEventListener('mouseup', this.onMouseUp);
        window.addEventListener('touchend', this.onTouchEnd);
    }

    stop() {
        this.canvas.removeEventListener('mousedown', this.onMouse);
        this.canvas.removeEventListener('mousemove', this.onMouse);
        this.canvas.removeEventListener('touchstart', this.onTouch);
        this.canvas.removeEventListener('touchmove', this.onTouch);
        window.removeEventListener('mouseup', this.onMouseUp);
        window.removeEventListener('touchend', this.onTouchEnd);
    }
}
