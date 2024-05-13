import binaryen from 'binaryen';
import { readFile, writeFile } from 'fs/promises';

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
                    [
                        module.local.get(1, binaryen.i32),
                        module.local.get(2, binaryen.i32),
                        module.local.get(3, binaryen.i32),
                    ],
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
    execFnNameToHandleMap: Map<string, number>
): void {
    const fnInfo = binaryen.getFunctionInfo(fn);
    console.log(`remapping opcodes in ${fnInfo.name}`);

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

                if (typeof constInfo.value === 'number' && constInfo.value >= 31400000 && constInfo.value < 31410000) {
                    const execFnTableEntry = constInfo.value - 31400000;

                    const execFnName = execFnTableEntryToNameMap.get(execFnTableEntry);
                    if (execFnName === undefined) throw new Error(`no execFn for table entry ${execFnTableEntry}`);

                    const execFnHandle = execFnNameToHandleMap.get(execFnName);
                    if (execFnHandle === undefined) throw new Error(`no execFn handle for ${execFnName}`);

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
    console.log(`replacing ${name}`);

    const fn = getFunctionLikeOne(module, name);
    const info = binaryen.getFunctionInfo(fn);

    module.removeFunction(info.name);
    module.addFunction(info.name, info.params, info.results, [], implementation);
}

async function main(filename: string): Promise<void> {
    const content = await readFile(filename);
    const module = binaryen.readBinary(new Uint8Array(content, content.byteLength));
    module.setFeatures(
        binaryen.Features.MVP | binaryen.Features.BulkMemory | binaryen.Features.Multivalue | binaryen.Features.SignExt
    );

    const elementsSegment = module.getElementSegmentByIndex(0);

    const execFnTableEntries = binaryen
        .getElementSegmentInfo(elementsSegment)
        .data.map((name, i): [number, string] => [i + 1, name])
        .filter(([i, name]) => name.includes('execFn_'));

    const execFnTableEntryToNameMap = new Map(execFnTableEntries);
    const execFnHandles = execFnTableEntries.map(([i, name], handle): [string, number] => [name, handle]);
    const execFnNameToHandleMap = new Map(execFnHandles);

    console.log('building dispatcher');
    const dispatcher = buildDispatcher(module, execFnHandles);

    replaceDispatcher(module, 'cpuPrvDispatchExecFnThumb', dispatcher);
    replaceDispatcher(module, 'cpuPrvDispatchExecFnArm', dispatcher);

    getFunctionLike(module, 'cpuDecodeArm').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMap)
    );

    getFunctionLike(module, 'cpuDecodeThumb').forEach((fnInfo) =>
        remapExecFunctions(module, fnInfo, execFnTableEntryToNameMap, execFnNameToHandleMap)
    );

    if (module.validate()) {
        await writeFile(filename, module.emitBinary());
        await writeFile(`${filename}.s`, module.emitText());
    }
}

if (process.argv.length < 3) {
    console.error('usage: main.ts <input.wasm>');
    process.exit(1);
}

main(process.argv[2]).catch((e) => {
    console.error(e);
    process.exit(1);
});
