/// <reference lib="webworker" />
import { EngineSettings } from '@common/engine/EngineSettings';

import { ClientMessageType } from './ClientMessage';
import { HostMessage, HostMessageType } from './HostMessage';
import { RpcClient } from './RpcClient';
import { RcpMethod, RpcArgsForMethod } from './rpc';

async function rpcInitialize({ module, settings }: RpcArgsForMethod<RcpMethod.initialize>): Promise<void> {
    console.log('worker initialized', module, settings);
}

async function rpcUpdateSettings(settings: EngineSettings): Promise<void> {
    console.log('updated engine settings', settings);
}

const rpcClient = new RpcClient((message, transferable) =>
    self.postMessage(message, transferable as Array<Transferable>),
);
rpcClient.register(RcpMethod.initialize, rpcInitialize).register(RcpMethod.updateSettings, rpcUpdateSettings);

async function onMessage(e: MessageEvent) {
    const message: HostMessage = e.data;

    switch (message.type) {
        case HostMessageType.rpcCall:
            void rpcClient.dispatch(message);
    }
}

addEventListener('message', onMessage);

postMessage({ type: ClientMessageType.ready });
