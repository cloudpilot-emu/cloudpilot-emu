import { Engine } from '@common/model/Engine';

const MAGIC_CLOUDPILOT = 0x20150103;
const MAGIC_UARM = 0x19800819;

export function identifySessionEngine(data: Uint8Array): Engine | undefined {
    if (data.length < 4) return undefined;

    const view = new DataView(data.buffer);
    const magic = view.getUint32(0, true);

    switch (magic) {
        case MAGIC_CLOUDPILOT:
            return Engine.cloudpilot;

        case MAGIC_UARM:
            return Engine.uarm;

        default:
            return undefined;
    }
}
