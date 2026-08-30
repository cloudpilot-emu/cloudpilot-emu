import binaryen from 'binaryen';
import { readFile, writeFile } from 'fs/promises';

function unmangleName(name: string): string {
    return name.replace(/(\\[0-9a-f]{2})/gi, (code) => String.fromCharCode(parseInt(code.substring(1), 16)));
}

function getFunctionLike(module: binaryen.Module, fragment: string): Array<binaryen.FunctionRef> {
    const numFunctions = module.getNumFunctions();
    const refs: Array<binaryen.FunctionRef> = [];

    for (let i = 0; i < numFunctions; i++) {
        const info = binaryen.getFunctionInfo(module.getFunctionByIndex(i));

        if (info.name.includes(fragment)) {
            refs.push(module.getFunctionByIndex(i));
        }
    }

    return refs;
}

function getFunctionLikeOne(module: binaryen.Module, fragment: string): binaryen.FunctionRef {
    const refs = getFunctionLike(module, fragment);

    return refs.length === 1 ? refs[0] : 0;
}

function buildDispatcher(module: binaryen.Module, execFnHandles: Array<[string, number]>): binaryen.ExpressionRef {
    const sortedHandles = execFnHandles.slice().sort(([, h1], [, h2]) => h1 - h2);

    if (sortedHandles.length - 1 !== sortedHandles[sortedHandles.length - 1][1]) {
        throw new Error('handle list has gaps!');
    }

    let block = module.block(
        'invalid',
        [
            module.switch(
                sortedHandles.map(([, handle]) => `op${handle}`),
                'invalid',
                module.local.get(0, binaryen.i32),
                undefined,
            ),
        ],
        undefined,
    );

    block = module.block('op0', [block, module.call('abort', [], binaryen.none), module.return()], undefined);

    for (let i = 0; i < sortedHandles.length; i++) {
        block = module.block(
            i < sortedHandles.length - 1 ? `op${sortedHandles[i + 1][1]}` : null,
            [
                block,
                module.call(
                    sortedHandles[i][0],
                    [module.local.get(1, binaryen.i32), module.local.get(2, binaryen.i32)],
                    binaryen.none,
                ),
                module.return(),
            ],
            undefined,
        );
    }

    return block;
}

function remapExecFunctions(
    module: binaryen.Module,
    fn: binaryen.FunctionRef,
    execFnTableEntryToNameMap: Map<number, string>,
    execFnNameToHandleMap: Map<string, number>,
    usedFunctions: Set<string>,
): void {
    const fnInfo = binaryen.getFunctionInfo(fn);

    console.log(`remapping opcodes in ${unmangleName(fnInfo.name)}`);

    const mapExpression = (e: binaryen.ExpressionRef): binaryen.ExpressionRef => {
        if (e === 0) return e;

        const info = binaryen.getExpressionInfo(e);

        switch (info.id) {
            case binaryen.ExpressionIds.Block: {
                const blockInfo = info as binaryen.BlockInfo;

                return module.block(blockInfo.name, blockInfo.children.map(mapExpression), blockInfo.type);
            }

            case binaryen.ExpressionIds.Return: {
                const returnInfo = info as binaryen.ReturnInfo;

                return module.return(mapExpression(returnInfo.value));
            }

            case binaryen.ExpressionIds.Const: {
                const constInfo = info as binaryen.ConstInfo;

                if (
                    typeof constInfo.value === 'number' &&
                    constInfo.value >>> 16 === 0x53ae &&
                    execFnTableEntryToNameMap.has(constInfo.value & 0xffff)
                ) {
                    const execFnTableEntry = constInfo.value & 0xffff;

                    const execFnName = execFnTableEntryToNameMap.get(execFnTableEntry);
                    if (execFnName === undefined) {
                        throw new Error(`no execFn for table entry ${execFnTableEntry}`);
                    }

                    const execFnHandle = execFnNameToHandleMap.get(execFnName);
                    if (execFnHandle === undefined) {
                        throw new Error(`no execFn handle for ${execFnName}`);
                    }

                    usedFunctions.add(execFnName);

                    return module.i32.const(execFnHandle);
                }

                return e;
            }

            case binaryen.ExpressionIds.Select: {
                const selectInfo = info as binaryen.SelectInfo;

                return module.select(
                    selectInfo.condition,
                    mapExpression(selectInfo.ifTrue),
                    mapExpression(selectInfo.ifFalse),
                    selectInfo.type,
                );
            }

            case binaryen.ExpressionIds.If: {
                const ifInfo = info as binaryen.IfInfo;

                return module.if(ifInfo.condition, mapExpression(ifInfo.ifTrue), mapExpression(ifInfo.ifFalse));
            }

            case binaryen.ExpressionIds.LocalSet: {
                const localSetInfo = info as binaryen.LocalSetInfo;

                return localSetInfo.isTee
                    ? module.local.tee(localSetInfo.index, mapExpression(localSetInfo.value), localSetInfo.type)
                    : module.local.set(localSetInfo.index, mapExpression(localSetInfo.value));
            }

            case binaryen.ExpressionIds.Loop: {
                const loopInfo = info as binaryen.LoopInfo;

                return module.loop(loopInfo.name, mapExpression(loopInfo.body));
            }

            default:
                return e;
        }
    };

    module.removeFunction(fnInfo.name);
    module.addFunction(fnInfo.name, fnInfo.params, fnInfo.results, fnInfo.vars, mapExpression(fnInfo.body));
}

function replaceDispatcher(module: binaryen.Module, name: string, implementation: binaryen.ExportRef): void {
    for (const fn of getFunctionLike(module, name)) {
        const info = binaryen.getFunctionInfo(fn);

        console.log(`replacing ${unmangleName(info.name)}`);

        module.removeFunction(info.name);
        module.addFunction(info.name, info.params, info.results, [], implementation);
    }
}

function isThumb(name: string): boolean {
    name = unmangleName(name);

    if (name.indexOf('_thumb_') >= 0) return true;
    if (/<(0|1), true/.test(name)) return true;

    return false;
}

function isMemorySystemMMU(name: string): boolean {
    if (name.indexOf('<') < 0) return true;
    return name.indexOf('<0') >= 0;
}

function isMemorySystemMPU(name: string): boolean {
    if (name.indexOf('<') < 0) return true;
    return name.indexOf('<1') >= 0;
}

async function main(input: string, output: string): Promise<void> {
    const content = await readFile(input);
    const module = binaryen.readBinary(new Uint8Array(content, content.byteLength));

    module.setFeatures(
        binaryen.Features.MVP |
            binaryen.Features.BulkMemory |
            binaryen.Features.Multivalue |
            binaryen.Features.SignExt |
            binaryen.Features.NontrappingFPToInt |
            binaryen.Features.TailCall |
            (1 << 19),
    );
    binaryen.setDebugInfo(true);

    const elementsSegment = module.getElementSegmentByIndex(0);

    const execFnTableEntries = binaryen
        .getElementSegmentInfo(elementsSegment)
        .data.map((name, i): [number, string] => [i + 1, name])
        .filter(([, name]) => name.includes('execFn_'));

    const execFnTableEntryToNameMap = new Map(execFnTableEntries);

    const execFnHandlesArmMmu = execFnTableEntries
        .filter(([, name]) => !isThumb(name) && isMemorySystemMMU(name))
        .map(([i, name], handle): [string, number] => [name, handle]);

    const execFnHandlesArmMpu = execFnTableEntries
        .filter(([, name]) => !isThumb(name) && isMemorySystemMPU(name))
        .map(([i, name], handle): [string, number] => [name, handle]);

    const execFnHandlesThumbMmu = execFnTableEntries
        .filter(([, name]) => isThumb(name) && isMemorySystemMMU(name))
        .map(([i, name], handle): [string, number] => [name, handle]);

    const execFnHandlesThumbMpu = execFnTableEntries
        .filter(([, name]) => isThumb(name) && isMemorySystemMPU(name))
        .map(([i, name], handle): [string, number] => [name, handle]);

    const execFnNameToHandleMapMmu = new Map([...execFnHandlesArmMmu, ...execFnHandlesThumbMmu]);
    const execFnNameToHandleMapMpu = new Map([...execFnHandlesArmMpu, ...execFnHandlesThumbMpu]);

    const usedFunctionsMmu = new Set<string>();
    const usedFunctionsMpu = new Set<string>();

    getFunctionLike(module, 'cpuPrvDecoderArm<0').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMapMmu, usedFunctionsMmu),
    );

    getFunctionLike(module, 'cpuPrvDecoderArm<1').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMapMpu, usedFunctionsMpu),
    );

    getFunctionLike(module, 'cpuPrvDecoderThumb<0').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMapMmu, usedFunctionsMmu),
    );

    getFunctionLike(module, 'cpuPrvDecoderThumb<1').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMapMpu, usedFunctionsMpu),
    );

    const usedHandlesArmMmu = Array.from(usedFunctionsMmu)
        .filter((name) => !isThumb(name) && isMemorySystemMMU(name))
        .map((name): [string, number] => [name, execFnNameToHandleMapMmu.get(name)!]);

    const usedHandlesArmMpu = Array.from(usedFunctionsMpu)
        .filter((name) => !isThumb(name) && isMemorySystemMPU(name))
        .map((name): [string, number] => [name, execFnNameToHandleMapMpu.get(name)!]);

    const usedHandlesThumbMmu = Array.from(usedFunctionsMmu)
        .filter((name) => isThumb(name) && isMemorySystemMMU(name))
        .map((name): [string, number] => [name, execFnNameToHandleMapMmu.get(name)!]);

    const usedHandlesThumbMpu = Array.from(usedFunctionsMpu)
        .filter((name) => isThumb(name) && isMemorySystemMPU(name))
        .map((name): [string, number] => [name, execFnNameToHandleMapMpu.get(name)!]);

    console.log(`building dispatcher for ${usedHandlesArmMmu.length} ARM/MMU functions`);
    const dispatcherArmMmu = buildDispatcher(module, usedHandlesArmMmu);

    console.log(`building dispatcher for ${usedHandlesArmMpu.length} ARM/MPU functions`);
    const dispatcherArmMpu = buildDispatcher(module, usedHandlesArmMpu);

    console.log(`building dispatcher for ${usedHandlesThumbMmu.length} thumb/MMU functions`);
    const dispatcherThumbMmu = buildDispatcher(module, usedHandlesThumbMmu);

    console.log(`building dispatcher for ${usedHandlesThumbMpu.length} thumb/MPU functions`);
    const dispatcherThumbMpu = buildDispatcher(module, usedHandlesThumbMpu);

    replaceDispatcher(module, 'cpuPrvDispatchExecFnArm<0', dispatcherArmMmu);
    replaceDispatcher(module, 'cpuPrvDispatchExecFnArm<1', dispatcherArmMpu);
    replaceDispatcher(module, 'cpuPrvDispatchExecFnThumb<0', dispatcherThumbMmu);
    replaceDispatcher(module, 'cpuPrvDispatchExecFnThumb<1', dispatcherThumbMpu);

    if (module.validate()) {
        await writeFile(output, module.emitBinary());
        await writeFile(`${output}.s`, module.emitText());
    }
}

if (process.argv.length < 4) {
    console.error('usage: main.ts <input.wasm> <output.wasm>');
    process.exit(1);
}

main(process.argv[2], process.argv[3]).catch((e) => {
    console.error(e);
    process.exit(1);
});
