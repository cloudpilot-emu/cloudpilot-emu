import { Cloudpilot, SuspendKind, VoidPtr } from './../helper/Cloudpilot';

import { AlertService } from './alert.service';
import { Injectable } from '@angular/core';
import { KvsService } from './kvs.service';
import { Event as Microevent } from 'microevent.ts';
import { normalizeProxyAddress } from '../helper/proxyAddress';
import { v4 as uuid } from 'uuid';

@Injectable({
    providedIn: 'root',
})
export class ProxyService {
    constructor(private kvsService: KvsService, private alertService: AlertService) {}

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

    private disconnect(socket: WebSocket): void {
        this.unbindListeners(socket);
        socket.close();

        this.sessionId = '';
        this.socket = undefined;
    }

    private bindListeners(socket: WebSocket): void {
        socket.addEventListener('open', this.onSocketOpen);
        socket.addEventListener('message', this.onSocketMessage);
        socket.addEventListener('error', this.onSocketError);
    }

    private unbindListeners(socket: WebSocket): void {
        socket.addEventListener('open', this.onSocketOpen);
        socket.addEventListener('message', this.onSocketMessage);
        socket.addEventListener('error', this.onSocketError);
    }

    private cancelConnectioon(): void {
        this.disconnect(this.socket!);
        this.alertService.errorMessage('Connection to proxy closed unexpectedly due to an error.');

        if (!this.cloudpilot.isSuspended()) return;

        switch (this.cloudpilot.getSuspendKind()) {
            case SuspendKind.networkConnect:
                this.cloudpilot.getSuspendContextNetworkConnect().Cancel();
                break;

            case SuspendKind.networkRpc:
                this.cloudpilot.getSuspendContextNetworkRpc().Cancel();
                break;

            default:
                break;
        }
    }

    private onSocketOpen = (): void => {
        if (!this.cloudpilot.isSuspended() || this.cloudpilot.getSuspendKind() !== SuspendKind.networkConnect) {
            console.error('ERROR: socket connected, but emulation is not waiting for connect');

            return;
        }

        this.sessionId = uuid();

        this.cloudpilot.getSuspendContextNetworkConnect().Resume(this.sessionId);

        this.resumeEvent.dispatch();
    };

    private onSocketMessage = (evt: MessageEvent): void => {
        if (!this.cloudpilot.isSuspended() || this.cloudpilot.getSuspendKind() !== SuspendKind.networkRpc) {
            console.error('ERROR: socket received message, but emulation is not waiting for rpc');

            return;
        }

        if (!ArrayBuffer.isView(evt.data)) {
            console.error('ERROR: socket received a text frame');

            this.disconnect(this.socket!);

            return;
        }

        const ptr: VoidPtr = this.cloudpilot.copyBuffer(evt.data as Uint8Array);

        this.cloudpilot.getSuspendContextNetworkRpc().ReceiveResponse(ptr, evt.data.byteLength);

        this.cloudpilot.freeBuffer(ptr);

        this.resumeEvent.dispatch();
    };

    private onSocketError = (evt: Event): void => {
        this.cancelConnectioon();

        console.error('ERROR: socket error', evt);
    };

    private onProxyDisconnect = (sessionId: string): void => {
        if (sessionId !== this.sessionId || !this.socket) return;

        this.disconnect(this.socket);
    };

    public resumeEvent = new Microevent<void>();

    private socket: WebSocket | undefined;
    private sessionId = '';
    private cloudpilot!: Cloudpilot;
}
