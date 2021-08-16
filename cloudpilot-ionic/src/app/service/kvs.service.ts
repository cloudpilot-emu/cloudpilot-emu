import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { Kvs } from '../model/Kvs';
import { Mutex } from 'async-mutex';
import { StorageService } from './storage.service';

const DEFAULTS: Kvs = {
    volume: 0.5,
    showStatistics: false,
    clipboardIntegration: false,
    networkRedirection: false,
    proxyServer: '',
    proxyCredentials: {},
    runHidden: false,
};

@Injectable({
    providedIn: 'root',
})
export class KvsService {
    constructor(private storageService: StorageService) {
        this.initializationPromise = this.startInitialiation();
    }

    async initialize(): Promise<void> {
        return this.initializationPromise;
    }

    public get kvs(): Kvs {
        return this.kvsProxy;
    }

    public set = (data: Partial<Kvs>) =>
        this.storageMutex.runExclusive(async () => {
            await this.storageService.kvsSet(data);

            for (const key of Object.keys(data)) {
                (this.rawKvs as any)[key] = data[key as keyof Kvs];
            }

            this.updateEvent.dispatch();
        });

    private async startInitialiation(): Promise<void> {
        const self = this;

        try {
            const kvs: Kvs = { ...DEFAULTS, ...(await this.storageService.kvsLoad()) };
            this.rawKvs = kvs;

            this.kvsProxy = new Proxy(kvs, {
                set<T extends keyof Kvs>(target: Kvs, key: T, value: Kvs[T]): boolean {
                    target[key] = value;

                    self.storageMutex.runExclusive(() => self.storageService.kvsSet({ [key]: value }));
                    self.updateEvent.dispatch();

                    return true;
                },
                deleteProperty(target: Kvs, key: keyof Kvs): boolean {
                    delete target[key];

                    self.storageMutex.runExclusive(() => self.storageService.kvsDelete(key));
                    self.updateEvent.dispatch();

                    return true;
                },
            });
        } catch (e) {
            console.error('failed to load KVS');
            console.error(e);

            this.kvsProxy = { ...DEFAULTS };
        }
    }

    updateEvent = new Event<void>();
    readonly mutex = new Mutex();

    private initializationPromise: Promise<void>;
    private storageMutex = new Mutex();
    private kvsProxy!: Kvs;
    private rawKvs!: Kvs;
}
