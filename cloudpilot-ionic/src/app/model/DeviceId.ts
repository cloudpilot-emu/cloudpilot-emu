export enum DeviceId {
    palmV = 'PalmV',
    iiic = 'PalmIIIc',
    m515 = 'PalmM515',
}

export function isColor(deviceId: DeviceId | undefined): boolean {
    return deviceId !== DeviceId.palmV;
}
