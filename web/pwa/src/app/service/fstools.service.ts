import { Injectable } from '@angular/core';
import { FsTools } from '@common/bridge/FSTools';
import wasmModule from '@native-fstools/fstools_web.wasm';

@Injectable({ providedIn: 'root' })
export class FsToolsService extends FsTools {
    constructor() {
        super(wasmModule);
    }
}
