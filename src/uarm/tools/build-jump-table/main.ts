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

        if (info.name.includes(fragment)) refs.push(module.getFunctionByIndex(i));
    }

    return refs;
}

function getFunctionLikeOne(module: binaryen.Module, fragment: string): binaryen.FunctionRef {
    const refs = getFunctionLike(module, fragment);

    return refs.length === 1 ? refs[0] : 0;
}

function buildDispatcher(module: binaryen.Module, execFnHandles: Array<[string, number]>): binaryen.ExpressionRef {
    let block = module.block(
        'invalid',
        [
            module.switch(
                execFnHandles.map(([name, handle]) => `op${handle}`),
                'invalid',
                module.local.get(0, binaryen.i32),
                undefined
            ),
        ],
        undefined
    );

    block = module.block('op0', [block, module.call('abort', [], binaryen.none), module.return()], undefined);

    for (let i = 0; i < execFnHandles.length; i++) {
        block = module.block(
            i < execFnHandles.length - 1 ? `op${i + 1}` : null,
            [
                block,
                module.call(
                    execFnHandles[i][0],
                    [module.local.get(1, binaryen.i32), module.local.get(2, binaryen.i32)],
                    binaryen.none
                ),
                module.return(),
            ],
            undefined
        );
    }

    return block;
}

function remapExecFunctions(
    module: binaryen.Module,
    fn: binaryen.FunctionRef,
    execFnTableEntryToNameMap: Map<number, string>,
    execFnNameToHandleMap: Map<string, number>,
    usedFunctions: Set<string>
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
                    if (execFnName === undefined) throw new Error(`no execFn for table entry ${execFnTableEntry}`);

                    const execFnHandle = execFnNameToHandleMap.get(execFnName);
                    if (execFnHandle === undefined) throw new Error(`no execFn handle for ${execFnName}`);

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
                    selectInfo.type
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

function isThumb(name: string) {
    name = unmangleName(name);

    if (name.indexOf('_thumb_') >= 0) return true;
    if (name.indexOf('<true') >= 0) return true;

    return false;
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
            (1 << 19)
    );
    binaryen.setDebugInfo(true);

    const elementsSegment = module.getElementSegmentByIndex(0);

    const execFnTableEntries = binaryen
        .getElementSegmentInfo(elementsSegment)
        .data.map((name, i): [number, string] => [i + 1, name])
        .filter(([i, name]) => name.includes('execFn_'));

    const execFnTableEntryToNameMap = new Map(execFnTableEntries);

    const execFnHandlesArm = execFnTableEntries
        .filter(([, name]) => !isThumb(name))
        .map(([i, name], handle): [string, number] => [name, handle]);

    const execFnHandlesThumb = execFnTableEntries
        .filter(([, name]) => isThumb(name))
        .map(([i, name], handle): [string, number] => [name, handle]);

    const execFnNameToHandleMap = new Map([...execFnHandlesThumb, ...execFnHandlesArm]);

    const usedFunctions = new Set<string>();

    getFunctionLike(module, 'cpuPrvDecoderArm').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMap, usedFunctions)
    );

    getFunctionLike(module, 'cpuPrvDecoderThumb').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMap, usedFunctions)
    );

    const usedHandlesArm = Array.from(usedFunctions)
        .filter((name) => !isThumb(name))
        .map((name): [string, number] => [name, execFnNameToHandleMap.get(name)!]);

    const usedHandlesThumb = Array.from(usedFunctions)
        .filter((name) => isThumb(name))
        .map((name): [string, number] => [name, execFnNameToHandleMap.get(name)!]);

    console.log(`building dispatcher for ${usedHandlesArm.length} ARM functions`);
    const dispatcherArm = buildDispatcher(module, usedHandlesArm);

    console.log(`building dispatcher for ${usedHandlesThumb.length} thumb functions`);
    const dispatcherThumb = buildDispatcher(module, usedHandlesThumb);

    replaceDispatcher(module, 'cpuPrvDispatchExecFnArm', dispatcherArm);
    replaceDispatcher(module, 'cpuPrvDispatchExecFnThumb', dispatcherThumb);

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
