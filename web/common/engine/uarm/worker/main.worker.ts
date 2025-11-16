/// <reference lib="webworker" />
// This is a hack to be able to use types that rely on DOM types.
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="./dom-stubs.worker.d.ts"/>
//
import { Uarm } from '@common/bridge/Uarm';

import { ClientMessage, ClientMessageType } from './ClientMessage';
import { Emulator } from './Emulator';
import { HostMessage, HostMessageType } from './HostMessage';
import { RpcClient } from './RpcClient';

let emulator: Emulator | undefined;

function unwrapEmulator(): Emulator {
    if (!emulator) throw new Error('worker not initialized');

    return emulator;
}

function dispatchMessage(message: ClientMessage, transferables?: Array<Transferable>): void {
    postMessage(message, transferables as Array<Transferable>);
}

const rpcClient = new RpcClient(dispatchMessage);
rpcClient
    .register('initialize', async ({ module, settings }) => {
        if (emulator) throw new Error('worker already initialized');

        const uarm = await Uarm.create(module);
        uarm.fatalErrorEvent.addHandler((error) =>
            dispatchMessage({ type: ClientMessageType.fatalError, error: error.message }),
        );

        emulator = new Emulator(uarm, settings);

        emulator.timesliceEvent.addHandler((props) =>
            dispatchMessage({ type: ClientMessageType.timeslice, ...props }, props.frame ? [props.frame] : undefined),
        );

        emulator.snapshotEvent.addHandler((snapshotResult) => {
            const [snapshot, transferables] = snapshotResult;

            dispatchMessage({ type: ClientMessageType.snapshot, snapshot }, transferables);
        });
    })
    .register('openSession', ({ rom, memory, nand, state, card }) =>
        unwrapEmulator().openSession(rom, nand, memory, state, card),
    )
    .register('start', () => unwrapEmulator().start())
    .register('stop', () => unwrapEmulator().stop())
    .register('takeSnapshot', (args, addTransferables) => {
        const snapshotResult = unwrapEmulator().takeSnapshot();
        if (!snapshotResult) throw new Error(`unable to take snapshot`);

        const [snapshot, transferrables] = snapshotResult;
        addTransferables(transferrables);

        return snapshot;
    })
    .register('refreshSnapshot', (snapshot, addTransferables) => {
        const [updatedSnapshot, transferables] = unwrapEmulator().refreshSnapshot(snapshot);
        addTransferables(transferables);

        return updatedSnapshot;
    })
    .register('waitForPendingSnapshot', () => unwrapEmulator().waitForPendingSnapshot())
    .register('getMemorySize', () => ({
        memory: unwrapEmulator().getMemorySize(),
        nand: unwrapEmulator().getNandSize(),
    }))
    .register('sdCardInsert', ({ data, key }) => unwrapEmulator().sdCardInsert(data, key))
    .register('sdCardEject', () => unwrapEmulator().sdCardEject())
    .register('getSdCardState', () => {
        if (!unwrapEmulator().sdCardInserted()) return { inserted: false };

        const key = unwrapEmulator().getSdCardKey();
        if (key === undefined) throw new Error('unreachable');

        return { inserted: true, key };
    })
    .register('installDb', (data) => unwrapEmulator().installDb(data))
    .register('backup', ({ includeRom }, addTransferables) => {
        const backup = unwrapEmulator().backup(includeRom);

        if (backup) addTransferables([backup.archive.buffer]);
        return backup;
    })
    .register('getSdCardData', (args, addTransferables) => {
        const data = unwrapEmulator().getSdCardData();

        if (data) addTransferables([data.buffer]);
        return data;
    });

async function onMessage(e: MessageEvent) {
    const message: HostMessage = e.data;

    switch (message.type) {
        case HostMessageType.rpcCall:
            void rpcClient.dispatch(message);
            break;

        case HostMessageType.updateSettings:
            emulator?.updateSettings(message.settings);
            break;

        case HostMessageType.buttonDown:
            emulator?.buttonDown(message.button);
            break;

        case HostMessageType.buttonUp:
            emulator?.buttonUp(message.button);
            break;

        case HostMessageType.penDown:
            emulator?.penDown(message.x, message.y);
            break;

        case HostMessageType.penUp:
            emulator?.penUp();
            break;

        case HostMessageType.returnFrame:
            emulator?.returnFrame(message.frame);
            break;

        case HostMessageType.returnSnapshot:
            emulator?.returnSnapshot(message.snapshot, message.success);
            break;

        case HostMessageType.setBackgrounded:
            emulator?.setBackgrounded(message.backgrounded);
            break;

        case HostMessageType.reset:
            emulator?.reset(message.resetType);
            break;

        default:
            message satisfies never;
    }
}

addEventListener('message', onMessage);
dispatchMessage({ type: ClientMessageType.ready });
