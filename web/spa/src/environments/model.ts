export interface Environment {
    production: boolean;
    dbName: string;
    releaseExtra?: string;
    localStorageSuffix?: string;
}
