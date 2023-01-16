import { Injectable } from '@angular/core';

export interface DragDropClient {
    handleDragDropEvent(e: DragEvent): void | Promise<void>;
}

@Injectable({ providedIn: 'root' })
export class DragDropService {
    constructor() {
        document.addEventListener('drop', this.onDrop);
        document.addEventListener('dragover', this.onDragOver);
    }

    registerClient(client: DragDropClient): void {
        this.currentClient = client;
    }

    unregisterClient(client: DragDropClient): void {
        if (client === this.currentClient) this.currentClient = undefined;
    }

    private onDragOver = (e: DragEvent): void => {
        e.preventDefault();

        if (e.dataTransfer) e.dataTransfer.dropEffect = this.currentClient ? 'copy' : 'none';
    };

    private onDrop = (e: DragEvent): void => {
        e.preventDefault();

        if (this.currentClient) void this.currentClient.handleDragDropEvent(e);
    };

    private currentClient: DragDropClient | undefined;
}
