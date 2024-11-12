import { Injectable, NgZone } from '@angular/core';
import { Cloudpilot, SuspendKind, VoidPtr } from '@common/bridge/Cloudpilot';
import { Mutex } from 'async-mutex';
import { Event } from 'microevent.ts';
import { NetworkBackend } from './network-backend/network-backend';
import { NetworkBackendFactory } from './network-backend/network-backend-factory.service';

@Injectable({ providedIn: 'root' })
export class NetworkService {
    constructor(
        private ngZone: NgZone,
        private backendFactory: NetworkBackendFactory,
    ) {}

    initialize(cloudpilot: Cloudpilot) {
        this.cloudpilot = cloudpilot;

        cloudpilot.proxyDisconnectEvent.addHandler(this.onProxyDisconnect);
    }

    isConnected(): boolean {
        return this.session !== undefined;
    }

    reset(): void {
        if (!this.session) return;

        this.session.backend.unexpectedCloseEvent.removeHandler(this.onUnexpectedBackendClose);
        this.session.resumeEvent.removeHandler(this.onSessionResume);

        this.session.terminate();

        this.ngZone.run(() => (this.session = undefined));
    }

    handleSuspend(): void {
        if (!this.cloudpilot) return;

        switch (this.cloudpilot.getSuspendKind()) {
            case SuspendKind.networkConnect:
                this.createNewSession();

                if (this.session) {
                    void this.session.handleConnect().then((success) => success || this.reset());
                } else {
                    if (this.cloudpilot.getSuspendKind() === SuspendKind.networkConnect) {
                        this.cloudpilot.getSuspendContextNetworkConnect().Cancel();
                    }
                }

                break;

            case SuspendKind.networkRpc:
                if (this.session) {
                    void this.session.handleRpc();
                } else {
                    if (this.cloudpilot.getSuspendKind() === SuspendKind.networkRpc) {
                        this.cloudpilot.getSuspendContextNetworkRpc().Cancel();
                    }
                }
                break;

            default:
                break;
        }
    }

    private createNewSession(): void {
        this.reset();

        this.ngZone.run(
            () => (this.session = NetworkSession.create(this.backendFactory.createBackend(), this.cloudpilot)),
        );
        if (!this.session) return;

        this.session.backend.unexpectedCloseEvent.addHandler(this.onUnexpectedBackendClose);
        this.session.resumeEvent.addHandler(this.onSessionResume);
    }

    private onProxyDisconnect = (): void => {
        this.reset();
    };

    private onUnexpectedBackendClose = () => this.reset();

    private onSessionResume = () => this.resumeEvent.dispatch();

    resumeEvent = new Event<void>();

    private cloudpilot: Cloudpilot | undefined;
    private session: NetworkSession | undefined;
}

class NetworkSession {
    static create(backend: NetworkBackend | undefined, cloudpilot?: Cloudpilot): NetworkSession | undefined {
        if (!cloudpilot || !backend) return undefined;

        return new NetworkSession(backend, cloudpilot);
    }

    private constructor(
        public backend: NetworkBackend,
        cloudpilot: Cloudpilot,
    ) {
        this.cloudpilot = cloudpilot;

        backend.rpcResultEvent.addHandler(this.onRpcResult);
    }

    terminate(): void {
        this.cloudpilot = undefined;
        this.backend.rpcResultEvent.removeHandler(this.onRpcResult);

        this.disconnect();
    }

    handleConnect = () =>
        this.mutex.runExclusive(async (): Promise<boolean> => {
            try {
                const connectSuccess = await this.backend.connect();

                if (this.cloudpilot?.getSuspendKind() === SuspendKind.networkConnect) {
                    connectSuccess
                        ? this.cloudpilot.getSuspendContextNetworkConnect().Resume()
                        : this.cloudpilot.getSuspendContextNetworkConnect().Cancel();
                }

                return connectSuccess;
            } catch (e) {
                console.error('failed to connect to backend', e);

                if (this.cloudpilot?.getSuspendKind() === SuspendKind.networkConnect) {
                    this.cloudpilot.getSuspendContextNetworkConnect().Cancel();
                }

                return false;
            }
        });

    handleRpc = () =>
        this.mutex.runExclusive(async () => {
            if (this.cloudpilot?.getSuspendKind() !== SuspendKind.networkRpc) return;

            const ctx = this.cloudpilot.getSuspendContextNetworkRpc();
            const buffer = this.cloudpilot.resolveBuffer(ctx.GetRequestData(), ctx.GetRequestSize());

            try {
                const rpcSuccess = await this.backend.handleRpc(buffer);

                if (!rpcSuccess && this.cloudpilot?.getSuspendKind() === SuspendKind.networkRpc) {
                    this.cloudpilot.getSuspendContextNetworkRpc().Cancel();
                }
            } catch (e) {
                console.error('failed to process rpc request', e);

                if (this.cloudpilot?.getSuspendKind() === SuspendKind.networkRpc) {
                    this.cloudpilot.getSuspendContextNetworkRpc().Cancel();
                }
            }
        });

    private disconnect = () =>
        void this.mutex.runExclusive(async () => {
            if (!this.backend) return;

            await this.backend.disconnect();
        });

    private onRpcResult = (rpcData: Uint8Array) => {
        if (!this.cloudpilot) return;

        if (this.cloudpilot.getSuspendKind() !== SuspendKind.networkRpc) {
            console.error('ERROR: socket received message, but emulation is not waiting for rpc');

            return;
        }

        const ptr: VoidPtr = this.cloudpilot.copyBuffer(rpcData);

        this.cloudpilot.getSuspendContextNetworkRpc().ReceiveResponse(ptr, rpcData.length);
        this.cloudpilot.freeBuffer(ptr);

        this.resumeEvent.dispatch();
    };

    resumeEvent = new Event<void>();

    private cloudpilot: Cloudpilot | undefined;
    private mutex = new Mutex();
}
