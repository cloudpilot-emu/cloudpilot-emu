import { Inject, Injectable } from '@angular/core';
import { CardSupportLevel, Cloudpilot, RomInfo, SessionImage } from '@common/bridge/Cloudpilot';
import { slotType } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';
import { SlotType } from '@common/model/SlotType';

import { TOKEN_CLOUDPILOT_INSTANCE } from './token';

@Injectable({ providedIn: 'root' })
export class NativeSupportService {
    constructor(@Inject(TOKEN_CLOUDPILOT_INSTANCE) private cloudpilot: Promise<Cloudpilot>) {}

    async ramSizeForDevice(device: DeviceId, romData: Uint8Array): Promise<number> {
        switch (device) {
            case DeviceId.te2:
            case DeviceId.frankene2: {
                const rominfo = (await this.cloudpilot).getRomInfo(romData);

                if (rominfo?.engine !== 'uarm') throw new Error(`${device} is OS5, but ROM is invalid`);

                return rominfo.recommendedRamSize;
            }

            default:
                return (await this.cloudpilot).minRamForDevice(device);
        }
    }

    serializeSessionImage<T>(sessionImage: Omit<SessionImage<T>, 'version'>): Promise<Uint8Array | undefined> {
        return this.cloudpilot.then((cp) => cp.serializeSessionImage(sessionImage));
    }

    deserializeSessionImage<T>(buffer: Uint8Array | undefined): Promise<SessionImage<T> | undefined> {
        return this.cloudpilot.then((cp) => cp.deserializeSessionImage(buffer));
    }

    async deviceSupportsCard(device: DeviceId, size: number): Promise<boolean> {
        const deviceSlotType = slotType(device);
        if (deviceSlotType === SlotType.none) return false;

        const supportLevel = (await this.cloudpilot).getCardSupportLevel(size);
        switch (supportLevel) {
            case CardSupportLevel.sdAndMs:
                return true;

            case CardSupportLevel.sdOnly:
                return deviceSlotType === SlotType.sdcard;

            default:
                return false;
        }
    }

    getRomInfo(rom: Uint8Array): Promise<RomInfo | undefined> {
        return this.cloudpilot.then((cp) => cp.getRomInfo(rom));
    }

    getCardSupportLevel(size: number): Promise<CardSupportLevel> {
        return this.cloudpilot.then((cp) => cp.getCardSupportLevel(size));
    }
}
