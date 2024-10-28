import { Injectable } from '@angular/core';
import { KvsService } from '../kvs.service';
import { AlertService } from '../alert.service';
import { FetchService } from '../fetch.service';
import { NetworkBackend } from './network-backend';
import { NetworkBackendProxy } from './network-backend-proxy';

@Injectable({ providedIn: 'root' })
export class NetworkBackendFactory {
    constructor(
        private kvsService: KvsService,
        private alertService: AlertService,
        private fetchService: FetchService,
    ) {}

    createBackendProxy(proxyServer: string = this.kvsService.kvs.proxyServer): NetworkBackendProxy {
        return new NetworkBackendProxy(proxyServer, this.alertService, this.fetchService);
    }

    createBackend(): NetworkBackend | undefined {
        if (!this.kvsService.kvs.networkRedirection) return undefined;

        return this.createBackendProxy();
    }
}
