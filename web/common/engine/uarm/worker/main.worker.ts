/// <reference lib="webworker" />
import { MessageType } from './message';

export {};

postMessage({ type: MessageType.ready });
