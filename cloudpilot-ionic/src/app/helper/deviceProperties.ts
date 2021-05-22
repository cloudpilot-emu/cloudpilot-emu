import { Dimensions, ScreenSize } from './../model/Dimensions';

import { DeviceId } from 'src/app/model/DeviceId';

export function deviceDimensions(deviceId: DeviceId): Dimensions {
    switch (deviceId) {
        case DeviceId.i710:
            return {
                screenSize: ScreenSize.screen320x320,
                width: 320,
                height: 320,
                silkscreenHeight: 120,
            };

        default:
            return {
                screenSize: ScreenSize.screen160x160,
                width: 160,
                height: 160,
                silkscreenHeight: 60,
            };
    }
}

export function isColor(deviceId: DeviceId | undefined): boolean {
    switch (deviceId) {
        case DeviceId.pilot:
        case DeviceId.palmPilot:
        case DeviceId.iii:
        case DeviceId.palmV:
        case DeviceId.palmVx:
        case DeviceId.iiix:
        case DeviceId.iiie:
        case DeviceId.m100:
        case DeviceId.m105:
        case DeviceId.m125:
        case DeviceId.m500:
            return false;

        default:
            return true;
    }
}

export function deviceName(deviceId: DeviceId): string {
    switch (deviceId) {
        case DeviceId.pilot:
            return 'Pilot';

        case DeviceId.palmPilot:
            return 'Palm Pilot';

        case DeviceId.iii:
            return 'Palm III';

        case DeviceId.m500:
            return 'Palm m500';

        case DeviceId.m505:
            return 'Palm m505';

        case DeviceId.m515:
            return 'Palm m515';

        case DeviceId.palmVx:
            return 'Palm Vx';

        case DeviceId.iiix:
            return 'Palm IIIx';

        case DeviceId.iiie:
            return 'Palm IIIe';

        case DeviceId.palmV:
            return 'Palm V';

        case DeviceId.iiic:
            return 'Palm IIIc';

        case DeviceId.m100:
            return 'Palm m100';

        case DeviceId.m105:
            return 'Palm m105';

        case DeviceId.m125:
            return 'Palm m125';

        case DeviceId.m130:
            return 'Palm m130';

        case DeviceId.i710:
            return 'Tungsten W';

        default:
            throw new Error('bad device ID');
    }
}
