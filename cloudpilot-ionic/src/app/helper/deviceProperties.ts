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
