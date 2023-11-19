export function prerender(image) {
    let cachedWidth;
    let cachedHeight;
    const canvas = document.createElement('canvas');

    return async (width, height) => {
        if (cachedWidth === width && cachedHeight === height) {
            return canvas;
        }

        canvas.width = width;
        canvas.height = height;

        const ctx = canvas.getContext('2d');
        if (!ctx) throw new Error('get a new browser');

        ctx.drawImage(await image, 0, 0, canvas.width, canvas.height);

        cachedWidth = width;
        cachedHeight = height;

        return canvas;
    };
}

export function loadImage(url) {
    return new Promise((resolve, reject) => {
        const image = new Image();

        image.onload = () => resolve(image);
        image.onerror = () => reject();

        image.src = url;
    });
}
