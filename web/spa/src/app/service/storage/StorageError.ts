export class StorageError extends Error {
    constructor(msg: string | undefined) {
        super(msg);
    }
}
