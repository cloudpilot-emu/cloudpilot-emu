export const enum MessageType {
    ready = 'ready',
}

export interface MessageReady {
    type: MessageType.ready;
}

export type Message = MessageReady;
