import { Event } from 'microevent.ts';
import { Injectable } from '@angular/core';
import { Kvs } from '@pwa/model/Kvs';
import { Mutex } from 'async-mutex';
import { StorageService } from './storage.service';
import { NativeAppService } from './native-app.service';

const DEFAULTS: Kvs = {
    volume: 0.5,
    showStatistics: false,
    clipboardIntegration: false,
    networkRedirection: false,
    networkRedirectionMode: NativeAppService.supportsNativeNetworkIntegration() ? 'native' : 'proxy',
    proxyServer: '',
    credentials: {},
    runHidden: false,
    autoLockUI: false,
    enableRemoteInstall: false,
    enableAudioOnFirstInteraction: false,
    snapshotIntegrityCheck: false,
};

@Injectable({
    providedIn: 'root',
})
export class KvsService {
    constructor(private storageService: StorageService) {
        this.initializationPromise = this.startInitialization().then(() => this.migrate());
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
                // eslint-disable-next-line @typescript-eslint/no-explicit-any
                (this.rawKvs as any)[key] = data[key as keyof Kvs];
            }

            this.updateEvent.dispatch();
        });

    private async startInitialization(): Promise<void> {
        // eslint-disable-next-line @typescript-eslint/no-this-alias
        const self = this;

        try {
            const kvs: Kvs = { ...DEFAULTS, ...(await this.storageService.kvsLoad()) };
            this.rawKvs = kvs;

            this.kvsProxy = new Proxy(kvs, {
                set<T extends keyof Kvs>(target: Kvs, key: T, value: Kvs[T]): boolean {
                    target[key] = value;

                    void self.storageMutex.runExclusive(async () => {
                        await self.storageService.kvsSet({ [key]: value });
                    });
                    self.updateEvent.dispatch();

                    return true;
                },
                deleteProperty(target: Kvs, key: keyof Kvs): boolean {
                    delete target[key];

                    void self.storageMutex.runExclusive(() => self.storageService.kvsDelete(key));
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

    private async migrate(): Promise<void> {
        switch (this.kvs.networkRedirectionMode) {
            case 'native':
                if (!NativeAppService.supportsNativeNetworkIntegration()) {
                    await this.set({ networkRedirectionMode: 'proxy', networkRedirection: false });
                }

                break;

            case 'proxy':
                break;

            default:
                await this.set({ networkRedirectionMode: 'proxy' });
        }
    }

    updateEvent = new Event<void>();
    readonly mutex = new Mutex();

    private initializationPromise: Promise<void>;
    private storageMutex = new Mutex();
    private kvsProxy!: Kvs;
    private rawKvs!: Kvs;
}
