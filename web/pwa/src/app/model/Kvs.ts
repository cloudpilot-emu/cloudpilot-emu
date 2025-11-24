export interface Credentials {
    username: string;
    password: string;
}

export type NetworkRedirectionMode = 'native' | 'proxy';

export interface Kvs {
    version?: string;
    previousVersion?: string;
    latestVersion?: string;
    volume: number;
    didShowInvitation?: boolean;
    showStatistics: boolean;
    clipboardIntegration: boolean;
    networkRedirection: boolean;
    networkRedirectionMode: NetworkRedirectionMode;
    proxyServer: string;
    credentials: Record<string, Credentials>;
    runHidden: boolean;
    autoLockUI: boolean;
    enableRemoteInstall: boolean;
    enableAudioOnFirstInteraction: boolean;
    snapshotIntegrityCheck: boolean;
    infoId?: number;
    ios174UpdateWarningId?: number;
    dontEmulateDPad?: boolean;
}
