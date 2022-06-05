export type PrerenderedImage = (width: number, height: number) => Promise<HTMLCanvasElement>;
export type SkinModuleType = typeof import('@common/skin');
export type LazyLoadingImage = () => Promise<HTMLImageElement>;
export type SkinImageType = keyof SkinModuleType;

export type Skin = Record<SkinImageType, PrerenderedImage>;

const SKIN_IMAGE_KEYS: Array<SkinImageType> = [
    'SILKSCREEN_V',
    'SILKSCREEN_M500',
    'SILKSCREEN_M515',
    'SILKSCREEN_IIIC',
    'SILKSCREEN_M100',
    'SILKSCREEN_M130',
    'SILKSCREEN_TUNGSTEN_W',
    'SILKSCREEN_PILOT',
    'SILKSCREEN_I705',
    'SILKSCREEN_PEG_S300',
    'SILKSCREEN_PEG_S500',
    'SILKSCREEN_PEG_T415',
    'SILKSCREEN_PEG_N610',
    'SILKSCREEN_PEG_T600',
    'HARD_BUTTONS_PALM_V',
    'HARD_BUTTONS_M515',
    'HARD_BUTTONS_M500',
    'HARD_BUTTONS_IIIC',
    'HARD_BUTTONS_IIIX',
    'HARD_BUTTONS_IIIE',
    'HARD_BUTTONS_M100',
    'HARD_BUTTONS_M125',
    'HARD_BUTTONS_M130',
    'HARD_BUTTONS_TUNGSTEN_W',
    'HARD_BUTTONS_PILOT',
    'HARD_BUTTONS_I705',
    'HARD_BUTTONS_HANDERA330',
    'HARD_BUTTONS_PEG_S300',
    'HARD_BUTTONS_PEG_S500',
    'HARD_BUTTONS_PEG_S320',
    'HARD_BUTTONS_PEG_T415',
    'HARD_BUTTONS_PEG_N610',
    'HARD_BUTTONS_PEG_N700',
    'HARD_BUTTONS_PEG_NR70',
];

function prerender(image: LazyLoadingImage): PrerenderedImage {
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

        ctx.drawImage(await image(), 0, 0, canvas.width, canvas.height);

        cachedWidth = width;
        cachedHeight = height;

        return canvas;
    };
}

export function loadSkin(skinModule: Promise<SkinModuleType>): Skin {
    function loadImage(key: SkinImageType): LazyLoadingImage {
        let deferredImage: Promise<HTMLImageElement> | undefined;

        return async () => {
            if (!deferredImage) {
                const svg = (await skinModule)[key];

                deferredImage = new Promise<HTMLImageElement>((resolve, reject) => {
                    const image = new Image();

                    image.onload = () => resolve(image);
                    image.onerror = () => reject();

                    image.src = `data:image/svg+xml;base64,${btoa(svg)}`;
                });
            }

            return deferredImage;
        };
    }

    const prepareImage = (key: SkinImageType) => prerender(loadImage(key));
    const skin: Skin = {} as any;

    for (const key of SKIN_IMAGE_KEYS) {
        skin[key] = prepareImage(key);
    }

    return skin;
}
