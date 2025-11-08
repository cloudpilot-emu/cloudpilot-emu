// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>

const MIME_TYPE_WASM = 'application/wasm';

export async function loadAndCompileModule(binaryUrl: string): Promise<WebAssembly.Module> {
    const response = await fetch(binaryUrl);
    if (!response.ok) {
        throw new Error(`request for ${binaryUrl} failed with ${response.status}: ${response.statusText}`);
    }

    const mimeType = response.headers.get('Content-Type') ?? 'no mime type';

    if (mimeType !== MIME_TYPE_WASM) {
        console.warn(`response for ${binaryUrl} has wrong mime type: expected ${MIME_TYPE_WASM}, got ${mimeType}`);
    }

    if (mimeType === MIME_TYPE_WASM && WebAssembly.compileStreaming) {
        return WebAssembly.compileStreaming(response);
    }

    const moduleData = await response.arrayBuffer();
    return WebAssembly.compile(moduleData);
}

export function cachedInstantiate(binaryUrl: string): EmscriptenModule['instantiateWasm'] {
    const wasmModule = loadAndCompileModule(binaryUrl);

    return (imports, callback) =>
        void wasmModule
            .then((module) => WebAssembly.instantiate(module, imports))
            .then((instance) => callback(instance));
}
