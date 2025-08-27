import { Cloudpilot, SuspendKind } from '@common/bridge/Cloudpilot';

import { AlertController } from '@ionic/angular';
import { AlertService } from './alert.service';
import { Injectable } from '@angular/core';
import { NativeAppService } from './native-app.service';

const READ_CLIPBOARD_TTL = 3000;
const E_PERMISSION_DENIED = new Error('permission denied');

@Injectable({
    providedIn: 'root',
})
export class ClipboardService {
    constructor(
        private alertController: AlertController,
        private alertService: AlertService,
        private nativeAppService: NativeAppService,
    ) {}

    isSupported(): boolean {
        return (
            (!!navigator.clipboard?.readText && !!navigator.clipboard?.writeText) ||
            NativeAppService.supportsNativeClipboard()
        );
    }

    handleSuspend(cloudpilot: Cloudpilot): void {
        switch (cloudpilot.getSuspendKind()) {
            case SuspendKind.clipboardCopy:
                void this.handleCopy(cloudpilot);
                break;

            case SuspendKind.clipboardPaste:
                void this.handlePaste(cloudpilot);
                break;
        }
    }

    private async handleCopy(cloudpilot: Cloudpilot): Promise<void> {
        const ctx = cloudpilot.getSuspendContextClipboardCopy();
        const clipboardContent = ctx.GetClipboardContent();

        if (NativeAppService.supportsNativeClipboard()) {
            try {
                await this.nativeAppService.clipboardWrite(clipboardContent);

                ctx.Resume();
            } catch (e) {
                console.error(e);

                ctx.Cancel();
            }

            return;
        }

        try {
            await this.tryCopyDirect(clipboardContent);

            ctx.Resume();

            return;
        } catch (e) {
            if (e === E_PERMISSION_DENIED) {
                ctx.Cancel();

                return this.msgPermissionDenied();
            }
        }

        try {
            await this.tryCopyWithInteraction(clipboardContent);

            ctx.Resume();
            // eslint-disable-next-line @typescript-eslint/no-unused-vars
        } catch (e) {
            await this.msgCopyFailed();

            ctx.Cancel();
        }
    }

    private async tryCopyDirect(clipboardText: string): Promise<void> {
        try {
            await navigator.clipboard.writeText(clipboardText);
        } catch (e) {
            if (await this.isPermissionDenied('clipboard-write' as PermissionName)) throw E_PERMISSION_DENIED;

            throw e;
        }
    }

    private tryCopyWithInteraction(clipboardText: string): Promise<boolean> {
        return new Promise(async (resolve, reject) => {
            try {
                const alert = await this.alertController.create({
                    header: 'Confirm Copy',
                    message: 'Please confirm that you want to copy to the host clipboard',
                    backdropDismiss: false,
                    buttons: [
                        {
                            text: 'Cancel',
                            handler: () => {
                                void alert.dismiss();

                                resolve(false);
                            },
                        },
                        {
                            text: 'Copy',
                            handler: async () => {
                                void alert.dismiss();

                                try {
                                    await navigator.clipboard.writeText(clipboardText);

                                    resolve(true);
                                } catch (e) {
                                    reject(e);
                                }
                            },
                        },
                    ],
                });

                void alert.present();
            } catch (e) {
                reject(e);
            }
        });
    }

    private async handlePaste(cloudpilot: Cloudpilot): Promise<void> {
        const ctx = cloudpilot.getSuspendContextClipboardPaste();

        if (NativeAppService.supportsNativeClipboard()) {
            try {
                ctx.Resume(await this.nativeAppService.clipboardRead());
            } catch (e) {
                console.error(e);
                ctx.Resume('');
            }

            return;
        }

        const clipboardContent = await this.tryPaste();
        ctx.Resume(clipboardContent ?? '');

        if (clipboardContent !== undefined) {
            this.clipboardContent = clipboardContent;
            this.lastClipboardReadAt = performance.now();
        }
    }

    private async tryPaste(): Promise<string | undefined> {
        if (performance.now() - this.lastClipboardReadAt < READ_CLIPBOARD_TTL) {
            return this.clipboardContent;
        }

        try {
            return await this.tryPasteDirect();
        } catch (e) {
            if (e === E_PERMISSION_DENIED) {
                await this.msgPermissionDenied();

                return undefined;
            }
        }

        try {
            return await this.tryPasteWithInteraction();
            // eslint-disable-next-line @typescript-eslint/no-unused-vars
        } catch (e) {
            await this.msgPasteFailed();

            return undefined;
        }
    }

    private async tryPasteDirect(): Promise<string> {
        try {
            return await navigator.clipboard.readText();
        } catch (e) {
            if (await this.isPermissionDenied('clipboard-read' as PermissionName)) throw E_PERMISSION_DENIED;

            throw e;
        }
    }

    private tryPasteWithInteraction(): Promise<string | undefined> {
        return new Promise(async (resolve, reject) => {
            try {
                const alert = await this.alertController.create({
                    header: 'Confirm Paste',
                    message: 'Please confirm that you want to paste from the host clipboard',
                    backdropDismiss: false,
                    buttons: [
                        {
                            text: 'Cancel',
                            handler: () => {
                                void alert.dismiss();

                                resolve(undefined);
                            },
                        },
                        {
                            text: 'Paste',
                            handler: async () => {
                                void alert.dismiss();

                                try {
                                    resolve(await navigator.clipboard.readText());
                                } catch (e) {
                                    reject(e);
                                }
                            },
                        },
                    ],
                });

                void alert.present();
            } catch (e) {
                reject(e);
            }
        });
    }

    private async isPermissionDenied(name: PermissionName): Promise<boolean> {
        if (navigator.permissions) {
            try {
                const status = await navigator.permissions.query({ name });

                return status?.state === 'denied';
                // eslint-disable-next-line @typescript-eslint/no-unused-vars
            } catch (e) {
                return false;
            }
        }

        return false;
    }

    private msgPermissionDenied(): Promise<void> {
        return this.alertService.errorMessage(`
        CloudpilotEmu has no permission to access the clipboard. Please check your browser's settings
        and grant the necessary permission to ${location.origin} .
    `);
    }

    private msgCopyFailed(): Promise<void> {
        return this.alertService.errorMessage('Failed to copy to the host clipboard.');
    }

    private msgPasteFailed(): Promise<void> {
        return this.alertService.errorMessage('Failed to paste from the host clipboard.');
    }

    private clipboardContent = '';
    private lastClipboardReadAt = performance.now();
}
