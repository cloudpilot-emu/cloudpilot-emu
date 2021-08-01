export interface Credentials {
    username: string;
    password: string;
}

export interface Kvs {
    version?: string;
    previousVersion?: string;
    latestVersion?: string;
    volume: number;
    didShowInvitation?: boolean;
    showStatistics: boolean;
    clipboardIntegration: boolean;
    networkRedirection: boolean;
    proxyServer: string;
    proxyCredentials: Record<string, Credentials>;
}
