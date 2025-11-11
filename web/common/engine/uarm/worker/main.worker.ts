// This is a nasty hack. The web worker has no Navigator type available, but the
// Emscripten typings insist on testing it for WebGPU support. The following
// stub installs a type alias that forces Navigator to never. We have to put it here
// as an ambient type in order to make sure that the stub is available when the
// Emscripten typings are evaluated further down the lane.
//
/// <reference lib="webworker" />
// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="./stub-navigator.worker.d.ts"/>
//
import { Uarm } from '@common/bridge/Uarm';

import { ClientMessage, ClientMessageType } from './ClientMessage';
import { Emulator } from './Emulator';
import { HostMessage, HostMessageType } from './HostMessage';
import { RpcClient } from './RpcClient';
import { RcpMethod } from './rpc';

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
    .register(RcpMethod.initialize, async ({ module, settings }) => {
        if (emulator) throw new Error('worker already initialized');

        const uarm = await Uarm.create(module);
        uarm.fatalErrorEvent.addHandler((error) =>
            dispatchMessage({ type: ClientMessageType.fatalError, error: error.message }),
        );

        emulator = new Emulator(uarm, settings);
    })
    .register(RcpMethod.openSession, ({ rom, memory, nand, state, card }) =>
        unwrapEmulator().openSession(rom, nand, memory, state, card),
    );

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
    }
}

addEventListener('message', onMessage);
dispatchMessage({ type: ClientMessageType.ready });
