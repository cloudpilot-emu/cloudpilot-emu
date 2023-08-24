export type InstantiateFunction = (
    imports: WebAssembly.Imports,
    callback: (instance: WebAssembly.Instance) => void,
) => void;

export function cachedInstantiate(binaryUrl: string): InstantiateFunction {
    const wasmModule = WebAssembly.compileStreaming
        ? WebAssembly.compileStreaming(fetch(binaryUrl))
        : fetch(binaryUrl)
              .then((response) => response.arrayBuffer())
              .then((binary) => WebAssembly.compile(binary));

    return async (imports, callback) => callback(await WebAssembly.instantiate(await wasmModule, imports));
}
