import { Injectable } from '@angular/core';
import { KvsService } from '../kvs.service';
import { AlertService } from '../alert.service';
import { FetchService } from '../fetch.service';
import { NetworkBackend } from './network-backend';
import { NetworkBackendProxy } from './network-backend-proxy';
import { NativeBackendNative } from './network-backend-native';
import { NativeAppService } from '../native-app.service';

@Injectable({ providedIn: 'root' })
export class NetworkBackendFactory {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private fetchService: FetchService,
        private nativeAppService: NativeAppService,
    ) {}

    createBackendProxy(proxyServer: string = this.kvsService.kvs.proxyServer): NetworkBackendProxy {
        return new NetworkBackendProxy(proxyServer, this.alertService, this.fetchService);
    }

    createBackend(): NetworkBackend | undefined {
        if (!this.kvsService.kvs.networkRedirection) return undefined;

        if (
            this.kvsService.kvs.networkRedirectionMode === 'native' &&
            NativeAppService.supportsNativeNetworkIntegration()
        ) {
            return new NativeBackendNative(this.nativeAppService);
        }

        return this.createBackendProxy();
    }
}
