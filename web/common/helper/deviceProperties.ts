import { DeviceId } from '@common/model/DeviceId';
import { Dimensions, ScreenSize } from '@common/model/Dimensions';
import { EngineType } from '@common/model/Engine';

import { SlotType } from '../model/SlotType';

export function deviceDimensions(deviceId: DeviceId): Dimensions {
    switch (deviceId) {
        case DeviceId.i710:
        case DeviceId.m520:
        case DeviceId.te2:
            return {
                screenSize: ScreenSize.screen320x320,
                width: 320,
                height: 320,
                silkscreenHeight: 120,
            };

        case DeviceId.handera330:
        case DeviceId.handera330c:
        case DeviceId.lp168:
            return {
                screenSize: ScreenSize.screen240x320,
                width: 240,
                height: 320,
                silkscreenHeight: 0,
            };

        case DeviceId.pegS300:
        case DeviceId.pegS320:
        case DeviceId.pegS500c:
            return {
                screenSize: ScreenSize.screen160x160,
                width: 160,
                height: 160,
                silkscreenHeight: 76,
            };

        case DeviceId.pegT600c:
        case DeviceId.pegT400:
        case DeviceId.pegT650c:
            return {
                screenSize: ScreenSize.screen320x320,
                width: 320,
                height: 320,
                silkscreenHeight: 140,
                silkscreenOvershoot: 10,
            };

        case DeviceId.pegN600c:
        case DeviceId.pegN700c:
            return {
                screenSize: ScreenSize.screen320x320,
                width: 320,
                height: 320,
                silkscreenHeight: 120,
            };

        case DeviceId.pegNR70:
        case DeviceId.frankene2:
            return {
                screenSize: ScreenSize.screen320x480,
                width: 320,
                height: 480,
                silkscreenHeight: 0,
            };

        case DeviceId.acerS11:
            return {
                screenSize: ScreenSize.screen160x160,
                width: 160,
                height: 160,
                silkscreenHeight: 60,
                siklscreenShift: 8,
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
        case DeviceId.iiixe:
        case DeviceId.iiie:
        case DeviceId.m100:
        case DeviceId.m105:
        case DeviceId.m125:
        case DeviceId.m500:
        case DeviceId.palmVII:
        case DeviceId.palmVIIEZ:
        case DeviceId.palmVIIx:
        case DeviceId.i705:
        case DeviceId.handera330:
        case DeviceId.pegS300:
        case DeviceId.pegS320:
        case DeviceId.pegT400:
        case DeviceId.acerS11:
            return false;

        default:
            return true;
    }
}

export function cpuClock(deviceId: DeviceId): number {
    switch (deviceId) {
        case DeviceId.palmPilot:
        case DeviceId.pilot:
        case DeviceId.iii:
        case DeviceId.palmVII:
            return 16;

        case DeviceId.palmVx:
        case DeviceId.palmV:
        case DeviceId.palmVIIEZ:
        case DeviceId.palmVIIx:
        case DeviceId.iiic:
        case DeviceId.iiix:
        case DeviceId.iiixe:
        case DeviceId.iiie:
        case DeviceId.m100:
        case DeviceId.m105:
        case DeviceId.pegS300:
        case DeviceId.pegS500c:
            return 16;

        case DeviceId.m500:
        case DeviceId.m505:
        case DeviceId.m515:
        case DeviceId.m520:
        case DeviceId.m125:
        case DeviceId.m130:
        case DeviceId.i705:
        case DeviceId.i710:
        case DeviceId.handera330:
        case DeviceId.handera330c:
        case DeviceId.pegS320:
        case DeviceId.pegT400:
        case DeviceId.pegN600c:
        case DeviceId.pegT600c:
        case DeviceId.pegN700c:
        case DeviceId.acerS11:
            return 33;

        case DeviceId.pegT650c:
        case DeviceId.pegNR70:
        case DeviceId.lp168:
            return 66;

        case DeviceId.te2:
        case DeviceId.frankene2:
            return 100;
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

        case DeviceId.m520:
            return 'Palm m520';

        case DeviceId.palmVx:
            return 'Palm Vx';

        case DeviceId.iiix:
            return 'Palm IIIx';

        case DeviceId.iiixe:
            return 'Palm IIIxe';

        case DeviceId.iiie:
            return 'Palm IIIe';

        case DeviceId.palmV:
            return 'Palm V';

        case DeviceId.palmVII:
            return 'Palm VII';

        case DeviceId.palmVIIEZ:
            return 'Palm VII EZ';

        case DeviceId.palmVIIx:
            return 'Palm VIIx';

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

        case DeviceId.i705:
            return 'Palm i705';

        case DeviceId.i710:
            return 'Tungsten W';

        case DeviceId.handera330:
            return 'Handera 330';

        case DeviceId.handera330c:
            return 'Handera 330c';

        case DeviceId.pegS300:
            return 'Sony PEG-S300';

        case DeviceId.pegS320:
            return 'Sony PEG-S320';

        case DeviceId.pegT400:
            return 'Sony PEG-T400 series';

        case DeviceId.pegN600c:
            return 'Sony PEG-N600C series';

        case DeviceId.pegT600c:
            return 'Sony PEG-T600C series';

        case DeviceId.pegN700c:
            return 'Sony PEG-N700C series';

        case DeviceId.pegT650c:
            return 'Sony PEG-T650C series';

        case DeviceId.pegNR70:
            return 'Sony PEG-NR70 series';

        case DeviceId.pegS500c:
            return 'Sony PEG-S500C series';

        case DeviceId.acerS11:
            return 'Acer S1x series';

        case DeviceId.lp168:
            return 'Legend P168';

        case DeviceId.te2:
            return 'Tungsten E2';

        case DeviceId.frankene2:
            return 'Tungsten E3';

        default:
            throw new Error('bad device ID');
    }
}

export function quirkNoPoweroff(deviceId: DeviceId) {
    switch (deviceId) {
        case DeviceId.i705:
        case DeviceId.i710:
        case DeviceId.pegNR70:
        case DeviceId.pegT650c:
        case DeviceId.lp168:
            return true;

        default:
            return false;
    }
}

export function slotType(deviceId: DeviceId) {
    switch (deviceId) {
        case DeviceId.m500:
        case DeviceId.m505:
        case DeviceId.m515:
        case DeviceId.m520:
        case DeviceId.m125:
        case DeviceId.m130:
        case DeviceId.i705:
        case DeviceId.i710:
        case DeviceId.handera330:
        case DeviceId.handera330c:
        case DeviceId.te2:
        case DeviceId.frankene2:
            return SlotType.sdcard;

        case DeviceId.pegS300:
        case DeviceId.pegS500c:
        case DeviceId.pegS320:
        case DeviceId.pegT400:
        case DeviceId.pegN600c:
        case DeviceId.pegT600c:
        case DeviceId.pegN700c:
        case DeviceId.pegT650c:
        case DeviceId.pegNR70:
        case DeviceId.acerS11:
            return SlotType.memorystick;

        default:
            return SlotType.none;
    }
}

export function engine(deviceId: DeviceId): EngineType {
    switch (deviceId) {
        case DeviceId.te2:
        case DeviceId.frankene2:
            return EngineType.uarm;

        default:
            return EngineType.cloudpilot;
    }
}

export function nandSize(deviceId: DeviceId): number | undefined {
    // blocks * [pages / block] + [bytes / page]
    return engine(deviceId) === EngineType.uarm ? 2048 * (2 << 4) * 528 : undefined;
}

export function ramSize(deviceId: DeviceId): number {
    switch (deviceId) {
        case DeviceId.te2:
            return 16 << 20;

        case DeviceId.frankene2:
            return 32 << 20;

        default:
            return -1;
    }
}
