import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { EngineType } from '@common/model/Engine';
import { SessionMetadata } from '@common/model/SessionMetadata';

export const TARGET_MIPS_DEFAULT = 100;
export const WARN_SLOWDOWN_THRESHOLD_DEFAULT = 0.5;
export const MAX_HOST_LOAD_DEFAULT = 1;
export const DISABLE_AUDIO_DEFAULT = false;

export interface SessionState {
    id: number;
    rom: string;
    device: DeviceId;
    ram: number;
    nand?: number;
    osVersion?: number;
    mountedCard?: number;
    lastLaunch?: number;
    wasResetForcefully: boolean;
}

export interface SessionSettingsCommon {
    name: string;
    deviceOrientation: DeviceOrientation;
}

export interface SessionSettingsCloudpilot extends SessionSettingsCommon {
    engine: EngineType.cloudpilot;

    hotsyncName: string;
    dontManageHotsyncName: boolean;
    speed: number;
}

export interface SessionSettingsUarm extends SessionSettingsCommon {
    engine: EngineType.uarm;

    disableAudio: boolean;
    targetMips: number;
    warnSlowdownThreshold: number;
    maxHostLoad: number;
}

export type SessionCommon = SessionSettingsCommon & SessionState;
export type SessionCloudpilot = SessionSettingsCloudpilot & SessionState;
export type SessionUarm = SessionSettingsUarm & SessionState;
export type Session = SessionCloudpilot | SessionUarm;
export type SessionSettings = SessionSettingsCloudpilot | SessionSettingsUarm;

type SessionForEngine<E extends EngineType> = E extends EngineType.cloudpilot ? SessionCloudpilot : SessionUarm;

export function fixmeAssertSessionHasEngine<E extends EngineType>(
    session: Session | undefined,
    engine: E,
): asserts session is SessionForEngine<E> | undefined {
    if (!session) return undefined;

    if (session.engine !== EngineType.cloudpilot) throw new Error(`FIXME: unsupported session engine ${engine}`);
}

export function buildSettings(
    settings: Partial<SessionSettingsCloudpilot> & { engine: EngineType.cloudpilot },
): SessionSettingsCloudpilot;
export function buildSettings(
    settings: Partial<SessionSettingsUarm> & { engine: EngineType.uarm },
): SessionSettingsUarm;
export function buildSettings(settings: Partial<SessionSettings> & { engine: EngineType }): SessionSettings;
export function buildSettings(settings: Partial<SessionSettings> & { engine: EngineType }): SessionSettings {
    switch (settings.engine) {
        case EngineType.cloudpilot:
            return {
                name: '',
                deviceOrientation: DeviceOrientation.portrait,
                hotsyncName: '',
                dontManageHotsyncName: false,
                speed: 1,
                ...settings,
            };

        case EngineType.uarm:
            return {
                name: '',
                deviceOrientation: DeviceOrientation.portrait,
                disableAudio: DISABLE_AUDIO_DEFAULT,
                maxHostLoad: MAX_HOST_LOAD_DEFAULT,
                targetMips: TARGET_MIPS_DEFAULT,
                warnSlowdownThreshold: WARN_SLOWDOWN_THRESHOLD_DEFAULT,
                ...settings,
            };

        default:
            throw new Error('unreachable');
    }
}

export function settingsFromSession(session: SessionCloudpilot): SessionSettingsCloudpilot;
export function settingsFromSession(session: SessionUarm): SessionSettingsUarm;
export function settingsFromSession(session: Session): SessionSettings;
export function settingsFromSession(session: Session): SessionSettings {
    switch (session.engine) {
        case EngineType.cloudpilot: {
            const { engine, name, deviceOrientation, hotsyncName, dontManageHotsyncName, speed } = session;

            return { engine, name, deviceOrientation, hotsyncName, dontManageHotsyncName, speed };
        }

        case EngineType.uarm: {
            const { engine, name, deviceOrientation, targetMips, disableAudio, warnSlowdownThreshold, maxHostLoad } =
                session;

            return {
                engine,
                name,
                deviceOrientation,
                targetMips,
                disableAudio,
                warnSlowdownThreshold,
                maxHostLoad,
            };
        }

        default:
            throw new Error('unreachable');
    }
}

export function mergeSettings(session: SessionCloudpilot, settings: SessionSettingsCloudpilot): SessionCloudpilot;
export function mergeSettings(session: SessionUarm, settings: SessionSettingsUarm): SessionUarm;
export function mergeSettings(sesstion: Session, settings: SessionSettings): Session;
export function mergeSettings(sesstion: Session, settings: SessionSettings): Session {
    if (sesstion.engine !== settings.engine) throw new Error('engine mismatch');

    return { ...sesstion, ...settings };
}

export function settingsFromMetadata(
    engine: EngineType.cloudpilot,
    metadata: SessionMetadata | undefined,
): SessionSettingsCloudpilot;
export function settingsFromMetadata(
    engine: EngineType.uarm,
    metadata: SessionMetadata | undefined,
): SessionSettingsUarm;
export function settingsFromMetadata(engine: EngineType, metadata: SessionMetadata | undefined): SessionSettings;
export function settingsFromMetadata(engine: EngineType, metadata: SessionMetadata | undefined): SessionSettings {
    switch (engine) {
        case EngineType.cloudpilot:
            return {
                engine,
                name: metadata?.name ?? '',
                deviceOrientation: metadata?.deviceOrientation ?? DeviceOrientation.portrait,
                hotsyncName: metadata?.hotsyncName ?? '',
                dontManageHotsyncName: metadata?.dontManageHotsyncName ?? false,
                speed: metadata?.speed ?? 1,
            };

        case EngineType.uarm:
            return {
                engine,
                name: metadata?.name ?? '',
                deviceOrientation: metadata?.deviceOrientation ?? DeviceOrientation.portrait,
                disableAudio: metadata?.disableAudio ?? DISABLE_AUDIO_DEFAULT,
                maxHostLoad: metadata?.maxHostLoad ?? MAX_HOST_LOAD_DEFAULT,
                targetMips: metadata?.targetMips ?? TARGET_MIPS_DEFAULT,
                warnSlowdownThreshold: metadata?.warnSlowdownThreshold ?? WARN_SLOWDOWN_THRESHOLD_DEFAULT,
            };

        default:
            throw new Error('unreachable');
    }
}
