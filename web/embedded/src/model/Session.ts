import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';

export interface Session {
    hotsyncName: string | undefined;
    speed: number;
    orientation: DeviceOrientation;
    runInBackground: boolean;
    disableDpad: boolean;
    deviceId: DeviceId;

    maxHostLoad: number;
    disablePcmAudio: boolean;
}
