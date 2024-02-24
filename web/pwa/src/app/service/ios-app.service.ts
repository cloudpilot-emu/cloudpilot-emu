import { Injectable } from '@angular/core';

declare global {
    interface Window {
        webkit?: {
            messageHandlers?: {
                getEnableAudioOnStart: {
                    postMessage(dummy: undefined): Promise<boolean>;
                };

                setEnableAudioOnStart: {
                    postMessage(value: boolean): Promise<boolean>;
                };
            };
        };
    }
}

@Injectable({ providedIn: 'root' })
export class IosAppService {
    isSupported(): boolean {
        return !!window.webkit?.messageHandlers;
    }

    getEnableAudioOnStart(): Promise<boolean> {
        if (!window.webkit?.messageHandlers) {
            throw new Error('not supported');
        }

        return window.webkit.messageHandlers.getEnableAudioOnStart.postMessage(undefined);
    }

    async setEnableAudioOnStart(value: boolean): Promise<void> {
        if (!window.webkit?.messageHandlers) {
            throw new Error('not supported');
        }

        await window.webkit.messageHandlers.setEnableAudioOnStart.postMessage(value);
    }
}
