// I am not sure why the ambient import is required here --- tsc does fine without it, but
// the webpack build is unable to resolve emscripten with a simple ES6 style import.
//
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="../../node_modules/@types/emscripten/index.d.ts"/>

export function cachedInstantiate(binaryUrl: string): EmscriptenModule['instantiateWasm'] {
    const wasmModule = WebAssembly.compileStreaming
        ? WebAssembly.compileStreaming(fetch(binaryUrl))
        : fetch(binaryUrl)
              .then((response) => response.arrayBuffer())
              .then((binary) => WebAssembly.compile(binary));

    return (imports, callback) =>
        void wasmModule
            .then((module) => WebAssembly.instantiate(module, imports))
            .then((instance) => callback(instance));
}
