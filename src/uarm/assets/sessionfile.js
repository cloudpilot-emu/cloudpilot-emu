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
                this.bridge = new module.Bridge();

                return module;
            });
    }

    async serializeSession(ramSize, deviceId, metadata, nor, nand, ram, savestate) {
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
                .SetMetadata(encodedMetadata.length, metadataPtr)
                .SetNor(nor.length, norPtr)
                .SetNand(nand.length, nandPtr)
                .SetMemory(ram.length, ramPtr)
                .SetRamSize(ramSize);

            if (savestate) sessionFile.SetSavestate(savestate.length, savestatePtr);

            if (!sessionFile.Serialize()) return undefined;

            return this.copyOut(sessionFile.GetSerializedSessionSize(), sessionFile.GetSerializedSession());
        } finally {
            this.free(metadataPtr);
            this.free(norPtr);
            this.free(nandPtr);
            this.free(ramPtr);
            this.free(savestatePtr);

            module.destroy(sessionFile);
        }
    }

    async deserializeSession(session) {
        const module = await this.modulePromise;

        const sessionFile = new module.SessionFile();
        let sessionPtr = this.copyIn(session);

        try {
            if (!sessionFile.Deserialize(session.length, sessionPtr)) return undefined;

            const deviceId = sessionFile.GetDeviceId();
            const ramSize = sessionFile.GetRamSize();
            const metadataBinary = this.copyOut(sessionFile.GetMetadataSize(), sessionFile.GetMetadata());
            const nor = this.copyOut(sessionFile.GetNorSize(), sessionFile.GetNor());
            const nand = this.copyOut(sessionFile.GetNandSize(), sessionFile.GetNand());
            const ram = this.copyOut(sessionFile.GetMemorySize(), sessionFile.GetMemory());
            const savestate = this.copyOut(sessionFile.GetSavestateSize(), sessionFile.GetSavestate());

            let metadata;
            if (metadataBinary) {
                try {
                    const decoder = new TextDecoder();
                    metadata = JSON.parse(decoder.decode(metadataBinary));
                } catch (e) {
                    console.error(`failed to parseMetadata`);
                }
            }

            return { deviceId, metadata, nor, nand, ram, savestate, ramSize };
        } finally {
            this.free(session);
            module.destroy(sessionFile);
        }
    }

    copyIn(buffer) {
        if (!buffer) return 0;

        const ptr = this.malloc(buffer.length);
        const ptrNaked = this.module.getPointer(ptr);

        this.module.HEAPU8.subarray(ptrNaked, ptrNaked + buffer.length).set(buffer);

        return ptr;
    }

    copyOut(size, ptr) {
        if (!ptr) return undefined;
        const ptrNaked = this.module.getPointer(ptr);

        const buffer = new Uint8Array(size);
        buffer.set(this.module.HEAPU8.subarray(ptrNaked, ptrNaked + size));

        return buffer;
    }

    free(ptr) {
        if (ptr) this.bridge.Free(ptr);
    }

    malloc(size) {
        return this.bridge.Malloc(size);
    }
}
