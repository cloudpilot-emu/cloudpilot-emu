export interface Environment {
    production: boolean;
    dbName: string;
    debug: boolean;
    releaseExtra?: string;
    localStorageSuffix?: string;
}
