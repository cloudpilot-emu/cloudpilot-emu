export class SessionFile {
    constructor(wasmModule, log) {
        this.modulePromise = wasmModule
            .then((module) =>
                createModule({
                    print: log,
                    printErr: log,
                    instantiateWasm: (imports, callback) => WebAssembly.instantiate(module, imports).then(callback),
                })
            )
            .then((module) => {
                this.module = module;
                this._malloc = module.cwrap('malloc', 'number', ['number']);
                this._free = module.cwrap('free', undefined, ['number']);

                return module;
            });
    }

    async serializeSession(deviceId, metadata, nor, nand, ram, savestate) {
        const module = await this.modulePromise;

        const encoder = new TextEncoder();
        const encodedMetadata = encoder.encode(JSON.stringify(metadata));
        const sessionFile = new module.SessionFile();

        let metadataPtr;
        let norPtr;
        let nandPtr;
        let ramPtr;
        let savestatePtr;

        try {
            metadataPtr = this.copyIn(encodedMetadata);
            norPtr = this.copyIn(nor);
            nandPtr = this.copyIn(nand);
            ramPtr = this.copyIn(ram);
            savestatePtr = this.copyIn(savestate);

            sessionFile
                .SetDeviceId(deviceId)
                .SetMetadata(metadata.length, this.module.wrapPointer(metadataPtr))
                .SetNor(nor.length, this.module.wrapPointer(norPtr))
                .SetNand(nand.length, this.module.wrapPointer(nandPtr))
                .SetRam(ram.length, this.module.wrapPointer(ramPtr));

            if (savestate) sessionFile.SetSavestate(savestate.length, savestatePtr);

            if (!sessionFile.Serialize()) return undefined;

            return this.copyOut(
                sessionFile.GetSerializedSessionSize(),
                this.module.getPointer(sessionFile.GetSerializedSession())
            );
        } finally {
            this.free(metadataPtr);
            this.free(norPtr);
            this.free(nandPtr);
            this.free(ramPtr);
            this.free(savestatePtr);

            module.destroy(sessionFile);
        }
    }

    copyIn(buffer) {
        if (!buffer) return 0;

        const ptr = this.malloc(buffer.length);
        this.module.HEAPU8.subarray(ptr, ptr + buffer.length).set(buffer);

        return ptr;
    }

    copyOut(size, ptr) {
        if (!ptr) return undefined;

        const buffer = new Uint8Array(size);
        buffer.set(this.module.HEAPU8.subarray(ptr, ptr + size));

        return buffer;
    }

    free(ptr) {
        if (ptr) this._free(ptr);
    }

    malloc(size) {
        return this._malloc(size);
    }
}
