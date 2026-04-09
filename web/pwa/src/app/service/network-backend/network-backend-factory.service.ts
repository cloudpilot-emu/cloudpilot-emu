import { Injectable } from '@angular/core';

import { AlertService } from '../alert.service';
import { FetchService } from '../fetch.service';
import { KvsService } from '../kvs.service';
import { PlatformService } from '../platform-service.service';
import { NetworkBackend } from './network-backend';
import { NativeBackendNative } from './network-backend-native';
import { NetworkBackendProxy } from './network-backend-proxy';

@Injectable({ providedIn: 'root' })
export class NetworkBackendFactory {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private fetchService: FetchService,
        private platformService: PlatformService,
    ) {}

    createBackendProxy(proxyServer: string = this.kvsService.kvs.proxyServer): NetworkBackendProxy {
        return new NetworkBackendProxy(proxyServer, this.alertService, this.fetchService);
    }

    createBackend(): NetworkBackend | undefined {
        if (!this.kvsService.kvs.networkRedirection) return undefined;

        if (
            this.kvsService.kvs.networkRedirectionMode === 'native' &&
            this.platformService.supportsNativeNetworkIntegration()
        ) {
            return new NativeBackendNative(this.platformService);
        }

        return this.createBackendProxy();
    }
}
