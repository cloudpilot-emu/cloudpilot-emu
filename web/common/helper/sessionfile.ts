import { EngineType } from '@common/model/EngineType';

const MAGIC_CLOUDPILOT = 0x20150103;
const MAGIC_UARM = 0x19800819;

export function identifySessionEngine(data: Uint8Array): EngineType | undefined {
    if (data.length < 4) return undefined;

    const view = new DataView(data.buffer, data.byteOffset);
    const magic = view.getUint32(0, true);

    switch (magic) {
        case MAGIC_CLOUDPILOT:
            return 'cloudpilot';

        case MAGIC_UARM:
            return 'uarm';

        default:
            return undefined;
    }
}
