import { PrerenderedImage, loadImage, prerender } from '@common/helper/image';

import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { svgToUrl } from './../helper/image';

export const enum SkinElement {
    HARD_BUTTONS_IIIC = 'hard-buttons/iiic.svg',
    HARD_BUTTONS_IIIE = 'hard-buttons/iiie.svg',
    HARD_BUTTONS_IIIX = 'hard-buttons/iiix.svg',
    HARD_BUTTONS_M100 = 'hard-buttons/m100.svg',
    HARD_BUTTONS_M125 = 'hard-buttons/m125.svg',
    HARD_BUTTONS_M130 = 'hard-buttons/m130.svg',
    HARD_BUTTONS_M500 = 'hard-buttons/m500.svg',
    HARD_BUTTONS_M515 = 'hard-buttons/m515.svg',
    HARD_BUTTONS_PILOT = 'hard-buttons/pilot.svg',
    HARD_BUTTONS_TUNGSTEN_W = 'hard-buttons/tungstenw.svg',
    HARD_BUTTONS_PALM_V = 'hard-buttons/v.svg',
    HARD_BUTTONS_I705 = 'hard-buttons/i705.svg',
    HARD_BUTTONS_HANDERA330 = 'hard-buttons/handera330.svg',
    HARD_BUTTONS_HANDERA330C = 'hard-buttons/handera330c.svg',
    HARD_BUTTONS_PEG_S300 = 'hard-buttons/clie-s300.svg',
    HARD_BUTTONS_PEG_S500 = 'hard-buttons/clie-s500.svg',
    HARD_BUTTONS_PEG_S320 = 'hard-buttons/clie-s320.svg',
    HARD_BUTTONS_PEG_T415 = 'hard-buttons/clie-t415.svg',
    HARD_BUTTONS_PEG_N610 = 'hard-buttons/clie-n610.svg',
    HARD_BUTTONS_PEG_N700 = 'hard-buttons/clie-n700.svg',
    HARD_BUTTONS_PEG_NR70 = 'hard-buttons/clie-nr70.svg',
    HARD_BUTTONS_ACER_S10 = 'hard-buttons/acer-s10.svg',
    SILKSCREEN_IIIC = 'silkscreen/iiic.svg',
    SILKSCREEN_M100 = 'silkscreen/m100.svg',
    SILKSCREEN_M130 = 'silkscreen/m130.svg',
    SILKSCREEN_M500 = 'silkscreen/m500.svg',
    SILKSCREEN_M515 = 'silkscreen/m515.svg',
    SILKSCREEN_PILOT = 'silkscreen/pilot.svg',
    SILKSCREEN_TUNGSTEN_W = 'silkscreen/tungstenw.svg',
    SILKSCREEN_V = 'silkscreen/v.svg',
    SILKSCREEN_I705 = 'silkscreen/i705.svg',
    SILKSCREEN_PEG_S300 = 'silkscreen/clie-s300.svg',
    SILKSCREEN_PEG_S500 = 'silkscreen/clie-s500.svg',
    SILKSCREEN_PEG_T415 = 'silkscreen/clie-t415.svg',
    SILKSCREEN_PEG_N610 = 'silkscreen/clie-n610.svg',
    SILKSCREEN_PEG_T600 = 'silkscreen/clie-t600.svg',
    SILKSCREEN_ACER_S10 = 'silkscreen/acer-s10.svg',
}

export class SkinLoader {
    constructor(private cloudpilot: Promise<Cloudpilot>) {}

    loadSkinElement(elt: SkinElement): PrerenderedImage {
        if (this.images.has(elt)) return this.images.get(elt)!;

        const skin: Promise<[string, HTMLImageElement]> = this.cloudpilot
            .then((cloudpilot) => cloudpilot.loadSkin(elt))
            .then((svg) =>
                svg
                    ? Promise.all([svg, loadImage(svgToUrl(svg))])
                    : Promise.reject(new Error(`failed to load skin ${elt}`))
            );

        const prerenderedImage = prerender(
            skin.then(([, image]) => image),
            skin.then(([svg]) => svg)
        );

        this.images.set(elt, prerenderedImage);

        return prerenderedImage;
    }

    private images = new Map<SkinElement, PrerenderedImage>();
}
