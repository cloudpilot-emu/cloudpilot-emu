import { Cloudpilot, SuspendContextClipboardPaste, SuspendKind } from '@common/Cloudpilot';

import { AlertController } from '@ionic/angular';
import { AlertService } from './alert.service';
import { Injectable } from '@angular/core';

const READ_CLIPBOARD_TTL = 3000;
const E_PERMISSION_DENIED = new Error('permission denied');

@Injectable({
    providedIn: 'root',
})
export class ClipboardService {
    constructor(private alertController: AlertController, private alertService: AlertService) {}

    isSupported(): boolean {
        return !!navigator.clipboard?.readText && !!navigator.clipboard?.writeText;
    }

    handleSuspend(cloudpilot: Cloudpilot): void {
        if (!cloudpilot.isSuspended()) return;

        switch (cloudpilot.getSuspendKind()) {
            case SuspendKind.clipboardCopy:
                this.handleCopy(cloudpilot);
                break;

            case SuspendKind.clipboardPaste:
                this.handlePaste(cloudpilot);
                break;
        }
    }

    private async handleCopy(cloudpilot: Cloudpilot): Promise<void> {
        const ctx = cloudpilot.getSuspendContextClipboardCopy();
        const clipboardContent = ctx.GetClipboardContent();

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
                            text: 'Copy',
                            handler: async () => {
                                alert.dismiss();

                                try {
                                    await navigator.clipboard.writeText(clipboardText);

                                    resolve(true);
                                } catch (e) {
                                    reject(e);
                                }
                            },
                        },
                        {
                            text: 'Cancel',
                            handler: () => {
                                alert.dismiss();

                                resolve(false);
                            },
                        },
                    ],
                });

                alert.present();
            } catch (e) {
                reject(e);
            }
        });
    }

    private async handlePaste(cloudpilot: Cloudpilot): Promise<void> {
        const clipboardContent = await this.tryPaste();

        const ctx = cloudpilot.getSuspendContextClipboardPaste();
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
                            text: 'Paste',
                            handler: async () => {
                                alert.dismiss();

                                try {
                                    resolve(await navigator.clipboard.readText());
                                } catch (e) {
                                    reject(e);
                                }
                            },
                        },
                        {
                            text: 'Cancel',
                            handler: () => {
                                alert.dismiss();

                                resolve(undefined);
                            },
                        },
                    ],
                });

                alert.present();
            } catch (e) {
                reject(e);
            }
        });
    }

    private async handlePasteSafari(ctx: SuspendContextClipboardPaste): Promise<void> {
        const alert = await this.alertController.create({
            header: 'Confirm Paste',
            message: 'Please confirm that you want to paste from the host clipboard.',
            backdropDismiss: false,
            buttons: [
                {
                    text: 'Paste',
                    handler: async () => {
                        try {
                            this.clipboardContent = await navigator.clipboard.readText();
                            this.lastClipboardReadAt = performance.now();

                            alert.dismiss();

                            ctx.Resume(this.clipboardContent);
                        } catch (e) {
                            alert.dismiss();

                            await this.alertService.errorMessage('Paste from host clipboard failed.');

                            ctx.Resume('');
                        }
                    },
                },
                {
                    text: 'Cancel',
                    handler: () => {
                        alert.dismiss();

                        ctx.Resume('');
                    },
                },
            ],
        });

        await alert.present();
    }

    private async isPermissionDenied(name: PermissionName): Promise<boolean> {
        if (navigator.permissions) {
            try {
                const status = await navigator.permissions.query({ name });

                return status?.state === 'denied';
            } catch (e) {
                return false;
            }
        }

        return false;
    }

    private msgPermissionDenied(): Promise<void> {
        return this.alertService.errorMessage(`
        Cloudpilot has no permission to access the clipboard. Please check your browser's settings
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
