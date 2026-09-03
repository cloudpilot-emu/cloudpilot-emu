import { DeviceId } from '@common/model/DeviceId';
import { ScreenSize } from '@common/model/Dimensions';
import { DeviceType5, DisplayMode } from '@native-common/index';

export function screenSizeToDisplayMode(screenSize: ScreenSize): DisplayMode {
    switch (screenSize) {
        case ScreenSize.screen160x160:
            return DisplayMode.mode_160x160;

        case ScreenSize.screen160x220:
            return DisplayMode.mode_160x220;

        case ScreenSize.screen240x240:
            return DisplayMode.mode_240x240;

        case ScreenSize.screen240x320:
            return DisplayMode.mode_240x320;

        case ScreenSize.screen320x320:
            return DisplayMode.mode_320x320;

        case ScreenSize.screen320x480:
            return DisplayMode.mode_320x480;

        case ScreenSize.screen480x480:
            return DisplayMode.mode_480x480;

        case ScreenSize.screen480x720:
            return DisplayMode.mode_480x720;

        default:
            screenSize satisfies never;
            throw new Error('undefined');
    }
}

export function screenSizeFromDisplayMode(displayMode: DisplayMode): ScreenSize {
    switch (displayMode) {
        case DisplayMode.mode_160x160:
            return ScreenSize.screen160x160;

        case DisplayMode.mode_160x220:
            return ScreenSize.screen160x220;

        case DisplayMode.mode_240x240:
            return ScreenSize.screen240x240;

        case DisplayMode.mode_240x320:
            return ScreenSize.screen240x320;

        case DisplayMode.mode_320x320:
            return ScreenSize.screen320x320;

        case DisplayMode.mode_320x480:
            return ScreenSize.screen320x480;

        case DisplayMode.mode_480x480:
            return ScreenSize.screen480x480;

        case DisplayMode.mode_480x720:
            return ScreenSize.screen480x720;

        default:
            displayMode satisfies never;
            throw new Error('unreachable');
    }
}

export function deviceTypeUarmToDeviceId(deviceType: DeviceType5): DeviceId | undefined {
    switch (deviceType) {
        case DeviceType5.deviceTypeE2:
            return DeviceId.te2;

        case DeviceType5.deviceTypeFrankenE2:
            return DeviceId.frankene2;

        case DeviceType5.deviceTypePV:
            return DeviceId.repalmPV;

        default:
            return undefined;
    }
}

export function deviceTypeUarmFromDeviceId(deviceId: DeviceId): DeviceType5 {
    switch (deviceId) {
        case DeviceId.te2:
            return DeviceType5.deviceTypeE2;

        case DeviceId.frankene2:
            return DeviceType5.deviceTypeFrankenE2;

        case DeviceId.repalmPV:
            return DeviceType5.deviceTypePV;

        default:
            return DeviceType5.deviceTypeInvalid;
    }
}
