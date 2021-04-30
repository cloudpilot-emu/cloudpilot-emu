import { Injectable } from '@angular/core';
import { Kvs } from '../model/Kvs';
import { Mutex } from 'async-mutex';
import { StorageService } from './storage.service';

const DEFAULTS: Kvs = {
    volume: 0.5,
    showStatistics: false,
    clipboardIntegration: false,
};

@Injectable({
    providedIn: 'root',
})
export class KvsService {
    constructor(private storageService: StorageService) {}

    public async initialize(): Promise<void> {
        const self = this;
        try {
            const kvs: Kvs = { ...DEFAULTS, ...(await this.storageService.kvsLoad()) };

            this.kvsProxy = new Proxy(kvs, {
                set<T extends keyof Kvs>(target: Kvs, key: T, value: Kvs[T]): boolean {
                    target[key] = value;

                    self.mutex.runExclusive(() => self.storageService.kvsSet(key, value));

                    return true;
                },
                deleteProperty(target: Kvs, key: keyof Kvs): boolean {
                    delete target[key];

                    self.mutex.runExclusive(() => self.storageService.kvsDelete(key));

                    return true;
                },
            });
        } catch (e) {
            console.error('failed to load KVS');
            console.error(e);

            this.kvsProxy = { ...DEFAULTS };
        }
    }

    public get kvs(): Kvs {
        return this.kvsProxy;
    }

    private mutex = new Mutex();
    private kvsProxy!: Kvs;
}
