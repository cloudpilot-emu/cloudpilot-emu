import { normalizeProxyAddress } from '@pwa/helper/proxyAddress';
import { NetworkBackend } from './network-backend';
import { Event as Microevent } from 'microevent.ts';
import { AlertService } from '../alert.service';
import { FetchService } from '../fetch.service';

const CONNECT_TIMEOUT = 5000;
const LOADER_GRACE_TIME = 500;
const VERSION = 3;

interface HandshakeResponse {
    version: number;
    compatVersion: number;
    token: string;
}

export type HandshakeResult =
    | { status: 'failed' }
    | { status: 'version_mismatch' }
    | { status: 'success'; token: string };

export class NetworkBackendProxy implements NetworkBackend {
    constructor(
        private proxyServer: string,
        private alertService: AlertService,
        private fetchService: FetchService,
    ) {}

    async connect(): Promise<boolean> {
        const url = normalizeProxyAddress(this.proxyServer);

        if (!url) {
            await this.alertService.errorMessage(`Invalid proxy address: ${this.proxyServer}`);
            return false;
        }

        const handshakeResult = await this.handshake(LOADER_GRACE_TIME);

        switch (handshakeResult.status) {
            case 'failed':
                void this.alertService.errorMessage('Failed to connect to proxy.');
                return false;

            case 'version_mismatch':
                await this.alertService.proxyVersionMismatchError();
                return false;

            case 'success':
                break;

            default:
                throw new Error('unreachable');
        }

        const socket = new WebSocket(
            `${url.replace(/^http/, 'ws')}/network-proxy/connect?token=${encodeURIComponent(handshakeResult.token)}`,
        );
        socket.binaryType = 'arraybuffer';

        const connectTimeoutHandle = setTimeout(() => socket.close(), CONNECT_TIMEOUT);
        let onOpen: () => void;
        let onClose: () => void;
        let onError: (e: unknown) => void;

        try {
            await new Promise<void>((resolve, reject) => {
                onOpen = () => resolve();
                onError = (e) => reject(e);
                onClose = () => reject(new Error('connection timeout'));

                socket.addEventListener('open', onOpen);
                socket.addEventListener('error', onError);
                socket.addEventListener('close', onClose);
            });

            clearTimeout(connectTimeoutHandle);

            socket.addEventListener('error', this.onSocketError);
            socket.addEventListener('message', this.onSocketMessage);
            socket.addEventListener('close', this.onSocketClose);

            this.socket = socket;

            return true;
        } catch (e) {
            console.error('socket failed to connect', e);
            return false;
        } finally {
            socket.removeEventListener('open', onOpen!);
            socket.removeEventListener('error', onError!);
            socket.removeEventListener('close', onClose!);
        }
    }

    async disconnect(): Promise<void> {
        if (!this.socket) return;

        this.socket.removeEventListener('error', this.onSocketError);
        this.socket.removeEventListener('message', this.onSocketMessage);
        this.socket.removeEventListener('close', this.onSocketClose);

        this.socket.close();
        this.socket = undefined;
    }

    async handleRpc(rpcData: Uint8Array): Promise<boolean> {
        if (!this.socket) return false;

        this.socket.send(rpcData);

        return true;
    }

    async handshake(loaderDelay = -1): Promise<HandshakeResult> {
        const url = normalizeProxyAddress(this.proxyServer);
        if (!url) return { status: 'failed' };

        try {
            const response = await this.fetchService.fetch(`${url}/network-proxy/handshake`, {
                method: 'POST',
                loaderDelay,
            });

            if (!response.ok) {
                return { status: 'failed' };
            }

            const handshakeResponse: HandshakeResponse = await response.json();

            if (handshakeResponse.version < VERSION || handshakeResponse.compatVersion > VERSION) {
                return { status: 'version_mismatch' };
            }

            return { status: 'success', token: handshakeResponse.token };
            // eslint-disable-next-line @typescript-eslint/no-unused-vars
        } catch (e) {
            return { status: 'failed' };
        }
    }

    private onSocketMessage = (evt: MessageEvent): void => {
        if (!(evt.data instanceof ArrayBuffer)) {
            console.error('ERROR: socket received a text frame');
            void this.alertService.errorMessage('Connection to proxy closed unexpectedly due to an error.');

            void this.disconnect();
            this.unexpectedCloseEvent.dispatch(undefined);

            return;
        }

        this.rpcResultEvent.dispatch(new Uint8Array(evt.data));
    };

    private onSocketError = (evt: Event | undefined): void => {
        void this.alertService.errorMessage('Connection to proxy closed unexpectedly due to an error.');

        void this.disconnect();
        this.unexpectedCloseEvent.dispatch();

        console.error('ERROR: socket error', evt);
    };

    private onSocketClose = (): void => {
        void this.alertService.errorMessage('Connection to proxy closed unexpectedly');

        void this.disconnect();
        this.unexpectedCloseEvent.dispatch();
    };

    rpcResultEvent = new Microevent<Uint8Array>();
    unexpectedCloseEvent = new Microevent<void>();

    private socket: WebSocket | undefined;
}
