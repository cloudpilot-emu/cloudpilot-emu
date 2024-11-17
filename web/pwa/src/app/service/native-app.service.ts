import { Injectable, OnDestroy } from '@angular/core';
import { isIOSNative } from '@common/helper/browser';
import { Event } from 'microevent.ts';

interface NativeCallNetOpenSession {
    type: 'netOpenSession';
}

interface NativeCallNetCloseSession {
    type: 'netCloseSession';
    sessionId: number;
}

interface NativeCallNetDispatchRpc {
    type: 'netDispatchRpc';
    sessionId: number;
    rpcData: Array<number>;
}

interface NativeMessageNetRpcResult {
    type: 'netRpcResult';
    sessionId: number;
    rpcData: Array<number>;
}

type NativeMessage = NativeMessageNetRpcResult;

const RESUME_TIMEOUT_MSEC = 3000;

declare global {
    interface Window {
        webkit: {
            messageHandlers: {
                nativeCall: {
                    postMessage(payload: NativeCallNetOpenSession): Promise<number>;
                    postMessage(payload: NativeCallNetCloseSession): Promise<void>;
                    postMessage(payload: NativeCallNetDispatchRpc): Promise<number>;
                };
            };
        };

        __cpeCallFromNative?: (message: NativeMessage) => void;
        __cpeLastResume?: number;
    }
}

function determineApiVersion(): number {
    if (!isIOSNative) return 0;

    const match = navigator.userAgent.match(/cpe-native-api-(\d+)/);
    if (!match) return 0;

    const version = parseInt(match[1], 10);

    return isNaN(version) ? 0 : version;
}

const apiVersion = determineApiVersion();

const nativeMessage = new Event<NativeMessage>();
if (apiVersion > 0) {
    window.__cpeCallFromNative = (message) => nativeMessage.dispatch(message);
}

@Injectable({ providedIn: 'root' })
export class NativeAppService implements OnDestroy {
    constructor() {
        nativeMessage.addHandler(this.onNativeMessage);
    }

    ngOnDestroy(): void {
        nativeMessage.removeHandler(this.onNativeMessage);
    }

    netOpenSession(): Promise<number> {
        this.assertNativeNetworkIntegrationSupported();

        return window.webkit.messageHandlers.nativeCall.postMessage({ type: 'netOpenSession' });
    }

    netCloseSession(sessionId: number): Promise<void> {
        this.assertNativeNetworkIntegrationSupported();

        return window.webkit.messageHandlers.nativeCall.postMessage({ type: 'netCloseSession', sessionId });
    }

    netDispatchRpc(sessionId: number, rpcData: ArrayLike<number>): Promise<boolean> {
        this.assertNativeNetworkIntegrationSupported();

        return window.webkit.messageHandlers.nativeCall
            .postMessage({
                type: 'netDispatchRpc',
                sessionId,
                rpcData: Array.from(rpcData),
            })
            .then((x) => !!x);
    }

    isResumeFromBackground(): boolean {
        return window.__cpeLastResume !== undefined && Date.now() - window.__cpeLastResume <= RESUME_TIMEOUT_MSEC;
    }

    netRpcResult = new Event<{ sessionId: number; rpcData: Uint8Array }>();

    private onNativeMessage = (message: NativeMessage) => {
        switch (message.type) {
            case 'netRpcResult':
                this.netRpcResult.dispatch({ sessionId: message.sessionId, rpcData: new Uint8Array(message.rpcData) });
                break;

            default:
                console.error('unhandled native message', message);
        }
    };

    private assertNativeNetworkIntegrationSupported(): void {
        if (!NativeAppService.supportsNativeNetworkIntegration()) {
            throw new Error('native network integration not supported');
        }
    }

    static supportsNativeNetworkIntegration(): boolean {
        return isIOSNative && apiVersion > 0;
    }
}
