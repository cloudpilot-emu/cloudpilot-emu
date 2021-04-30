import { Cloudpilot, SuspendKind } from '../helper/Cloudpilot';

import { Injectable } from '@angular/core';

@Injectable({
    providedIn: 'root',
})
export class ClipboardService {
    constructor() {}

    isSupported(): boolean {
        return !!navigator.clipboard;
    }

    handleSuspend(cloudpilot: Cloudpilot): void {
        if (!cloudpilot.isSuspended()) return;

        switch (cloudpilot.getSuspendKind()) {
            case SuspendKind.clipboardCopy:
                return this.handleCopy(cloudpilot);

            case SuspendKind.clipboardPaste:
                return this.handlePaste(cloudpilot);
        }
    }

    private handleCopy(cloudpilot: Cloudpilot): void {
        const ctx = cloudpilot.getSuspendContextClipboardCopy();

        console.log('clipboard copy: ' + ctx.GetClipboardContent());

        ctx.Resume();
    }

    private handlePaste(cloudpilot: Cloudpilot): void {
        console.log('clipboard paste');
        cloudpilot.getSuspendContextClipboardPaste().Resume('fulpe');
    }
}
