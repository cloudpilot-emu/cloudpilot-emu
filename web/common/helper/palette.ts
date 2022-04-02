const BACKGROUND = [135, 135, 105];
const FOREGROUND = [0x0, 0x0, 0x0];

export const GRAYSCALE_PALETTE_RGBA = new Array(16)
    .fill(0)
    .map((_, i) => FOREGROUND.map((fg, j) => Math.floor((i * fg + (15 - i) * BACKGROUND[j]) / 15)))
    .map(([r, g, b]) => 0xff000000 | (b << 16) | (g << 8) | r);

export const GRAYSCALE_PALETTE_HEX = GRAYSCALE_PALETTE_RGBA.map(
    (x) => '#' + (x & 0xffffff).toString(16).padStart(6, '0').match(/(..?)/g)?.reverse().join('')
);
