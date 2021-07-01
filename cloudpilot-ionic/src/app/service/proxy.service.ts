import { Cloudpilot, SuspendKind, VoidPtr } from './../helper/Cloudpilot';
import { Injectable, NgZone } from '@angular/core';

import { AlertService } from './alert.service';
import { KvsService } from './kvs.service';
import { Event as Microevent } from 'microevent.ts';
import { normalizeProxyAddress } from '../helper/proxyAddress';
import { v4 as uuid } from 'uuid';

@Injectable({
    providedIn: 'root',
})
export class ProxyService {
    constructor(private kvsService: KvsService, private alertService: AlertService, private ngZone: NgZone) {}

    initialize(cloudpilot: Cloudpilot) {
        this.cloudpilot = cloudpilot;

        cloudpilot.proxyDisconnectEvent.addHandler(this.onProxyDisconnect);
    }

    reset(): void {
        if (this.socket) this.disconnect(this.socket);
    }

    handleSuspend(): void {
        if (!this.cloudpilot) return;

        if (!this.cloudpilot.isSuspended()) return;

        switch (this.cloudpilot.getSuspendKind()) {
            case SuspendKind.networkConnect:
                this.handleConnect();
                break;

            case SuspendKind.networkRpc:
                this.handleRpc();
                break;

            default:
                break;
        }
    }

    private async handleConnect(): Promise<void> {
        if (this.socket) this.disconnect(this.socket);

        const url = normalizeProxyAddress(this.kvsService.kvs.proxyServer);

        if (!url) {
            await this.alertService.errorMessage(`Invalid proxy address: ${this.kvsService.kvs.proxyServer}`);

            this.cloudpilot.getSuspendContextNetworkConnect().Cancel();

            return;
        }

        this.socket = new WebSocket(url);
        this.socket.binaryType = 'arraybuffer';

        this.bindListeners(this.socket);
    }

    private handleRpc(): void {
        const ctx = this.cloudpilot.getSuspendContextNetworkRpc();

        if (!this.socket) {
            ctx.Cancel();

            return;
        }

        this.socket.send(this.cloudpilot.resolveBuffer(ctx.GetRequestData(), ctx.GetRequestSize()));
    }

    private disconnect(socket: WebSocket | undefined): void {
        if (socket) {
            this.closing = true;
            this.unbindListeners(socket);
            socket.close();
        }

        this.sessionId = '';
        this.socket = undefined;
    }

    private bindListeners(socket: WebSocket): void {
        socket.addEventListener('open', this.onSocketOpen);
        socket.addEventListener('error', this.onSocketError);
        socket.addEventListener('close', this.onSocketClose);

        this.ngZone.runOutsideAngular(() => socket.addEventListener('message', this.onSocketMessage));
    }

    private unbindListeners(socket: WebSocket): void {
        socket.removeEventListener('message', this.onSocketMessage);
        socket.removeEventListener('open', this.onSocketOpen);
        socket.removeEventListener('error', this.onSocketError);
        socket.removeEventListener('close', this.onSocketClose);
    }

    private cancelSuspend(): void {
        if (!this.cloudpilot.isSuspended()) return;

        switch (this.cloudpilot.getSuspendKind()) {
            case SuspendKind.networkConnect:
            case SuspendKind.networkRpc:
                this.cloudpilot.cancelSuspend();
                break;

            default:
                break;
        }
    }

    private onSocketOpen = (): void => {
        if (!this.cloudpilot.isSuspended() || this.cloudpilot.getSuspendKind() !== SuspendKind.networkConnect) {
            console.error('ERROR: socket connected, but emulation is not waiting for connect');

            this.disconnect(this.socket);

            return;
        }

        this.sessionId = uuid();
        this.cloudpilot.getSuspendContextNetworkConnect().Resume(this.sessionId);

        this.closing = false;

        this.resumeEvent.dispatch();
    };

    private onSocketMessage = (evt: MessageEvent): void => {
        if (!this.cloudpilot.isSuspended() || this.cloudpilot.getSuspendKind() !== SuspendKind.networkRpc) {
            console.error('ERROR: socket received message, but emulation is not waiting for rpc');

            return;
        }

        if (!(evt.data instanceof ArrayBuffer)) {
            console.error('ERROR: socket received a text frame');
            this.disconnect(this.socket);
            this.alertService.errorMessage('Connection to proxy closed unexpectedly due to an error.');

            this.cancelSuspend();

            return;
        }

        const ptr: VoidPtr = this.cloudpilot.copyBuffer(new Uint8Array(evt.data));

        this.cloudpilot.getSuspendContextNetworkRpc().ReceiveResponse(ptr, evt.data.byteLength);
        this.cloudpilot.freeBuffer(ptr);

        this.resumeEvent.dispatch();
    };

    private onSocketError = (evt: Event): void => {
        if (this.cloudpilot.isSuspended() && this.cloudpilot.getSuspendKind() === SuspendKind.networkConnect) {
            this.alertService.errorMessage('Failed to connect to proxy.');
        } else {
            this.alertService.errorMessage('Connection to proxy closed unexpectedly due to an error.');
        }

        this.disconnect(this.socket);
        this.cancelSuspend();

        console.error('ERROR: socket error', evt);
    };

    private onSocketClose = (): void => {
        if (!this.closing) {
            this.alertService.errorMessage('Connection to proxy closed unexpectedly');
        }

        this.cancelSuspend();
        this.closing = false;
    };

    private onProxyDisconnect = (sessionId: string): void => {
        if (sessionId !== this.sessionId || !this.socket) return;

        this.disconnect(this.socket);
    };

    public resumeEvent = new Microevent<void>();

    private socket: WebSocket | undefined;
    private sessionId = '';
    private cloudpilot!: Cloudpilot;
    private closing = false;
}
