import { isIOS, isSafari } from './browser';

export type LazyLoadingImage = () => Promise<HTMLImageElement>;
export type PrerenderedImage = (width: number, height: number) => Promise<HTMLCanvasElement>;

export function prerender(image: Promise<HTMLImageElement>, source: string | Promise<string>): PrerenderedImage {
    let cachedWidth: number;
    let cachedHeight: number;
    const canvas = document.createElement('canvas');

    return async (width: number, height: number) => {
        if (cachedWidth === width && cachedHeight === height) {
            return canvas;
        }

        canvas.width = width;
        canvas.height = height;

        const ctx = canvas.getContext('2d');
        if (!ctx) throw new Error('get a new browser');

        ctx.drawImage(await image, 0, 0, canvas.width, canvas.height);

        if ((isSafari || isIOS) && (await source).indexOf('data:image') >= 0) {
            // Safari has a bug that renders embedded PNGs only on second draw.
            await new Promise((resolve) => setTimeout(resolve, 0));
            ctx.drawImage(await image, 0, 0, canvas.width, canvas.height);
        }

        cachedWidth = width;
        cachedHeight = height;

        return canvas;
    };
}

export function loadImage(url: string): Promise<HTMLImageElement> {
    return new Promise<HTMLImageElement>((resolve, reject) => {
        const image = new Image();

        image.onload = () => resolve(image);
        image.onerror = () => reject();

        image.src = url;
    });
}

export const svgToUrl = (svg: string): string => `data:image/svg+xml;base64,${btoa(svg)}`;
