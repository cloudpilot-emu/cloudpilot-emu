export interface VfsClipboardItem {
    name: string;
    path: string;
}

export type ClipboardOperation = 'copy' | 'cut';

export interface VfsClipboard {
    operation: ClipboardOperation;
    storageId: string;
    prefix: string;
    items: Array<VfsClipboardItem>;
}
