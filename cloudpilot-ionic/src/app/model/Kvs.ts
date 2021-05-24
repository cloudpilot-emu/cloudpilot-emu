export interface Kvs {
    version?: string;
    previousVersion?: string;
    latestVersion?: string;
    volume: number;
    didShowInvitation?: boolean;
    showStatistics: boolean;
    clipboardIntegration: boolean;
}
