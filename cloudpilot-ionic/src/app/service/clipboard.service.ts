import {
    Cloudpilot,
    SuspendContextClipboardCopy,
    SuspendContextClipboardPaste,
    SuspendKind,
} from '../helper/Cloudpilot';

import { AlertController } from '@ionic/angular';
import { AlertService } from 'src/app/service/alert.service';
import { Injectable } from '@angular/core';
import { isSafari } from './../helper/browser';

const READ_CLIPBOARD_TTL = 3000;

@Injectable({
    providedIn: 'root',
})
export class ClipboardService {
    constructor(private alertController: AlertController, private alertService: AlertService) {}

    isSupported(): boolean {
        return !!navigator.clipboard?.readText;
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

        if (isSafari) {
            await this.handleCopySafari(ctx);
        } else {
            try {
                await navigator.clipboard.writeText(ctx.GetClipboardContent());

                ctx.Resume();
            } catch (e) {
                await this.alertService.errorMessage('Copy to host clipboard failed.');

                ctx.Cancel();
            }
        }
    }

    private async handleCopySafari(ctx: SuspendContextClipboardCopy) {
        const alert = await this.alertController.create({
            header: 'Confirm Copy',
            message: 'Please confirm that you want to copy to the host clipboard',
            backdropDismiss: false,
            buttons: [
                {
                    text: 'Copy',
                    handler: async () => {
                        try {
                            await navigator.clipboard.writeText(ctx.GetClipboardContent());

                            alert.dismiss();

                            ctx.Resume();
                        } catch (e) {
                            alert.dismiss();

                            await this.alertService.errorMessage('Copy to host clipboard failed.');

                            ctx.Cancel();
                        }
                    },
                },
                {
                    text: 'Cancel',
                    handler: () => {
                        alert.dismiss();
                        ctx.Cancel();
                    },
                },
            ],
        });

        await alert.present();
    }

    private async handlePaste(cloudpilot: Cloudpilot): Promise<void> {
        const ctx = cloudpilot.getSuspendContextClipboardPaste();

        if (performance.now() - this.lastClipboardReadAt < READ_CLIPBOARD_TTL) {
            ctx.Resume(this.clipboardContent);
        } else if (isSafari) {
            await this.handlePasteSafari(ctx);
        } else {
            try {
                this.clipboardContent = await navigator.clipboard.readText();
                this.lastClipboardReadAt = performance.now();

                ctx.Resume(this.clipboardContent);
            } catch (e) {
                await this.alertService.errorMessage('Paste from host clipboard failed.');

                ctx.Cancel();
            }
        }
    }

    private async handlePasteSafari(ctx: SuspendContextClipboardPaste): Promise<void> {
        const alert = await this.alertController.create({
            header: 'Confirm Paste',
            message: 'Please confirm that you want to paste from the host clipboard',
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

    private clipboardContent = '';
    private lastClipboardReadAt = performance.now();
}
