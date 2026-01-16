import { CloudpilotEvent } from './Event';

/**
 * Emitted by the emulator.
 */
export interface ReceivePayload {
    /**
     * Serial data sent by PalmOS. This can be `undefined` in order to
     * communicate a frame boundary or the absence of a frame response
     * in 'sync' mode.
     */
    data: Uint8Array | undefined;

    /**
     * Marks a transmission frame as complete. This is always `true` in
     * 'simple' mode.
     */
    isFrameComplete: boolean;
}

export interface SerialPort {
    /**
     * Send serial data to the emulator.
     *
     * @param data Serial data to transmit (or undefined in order to communicate
     * a frame boundary or response timeout in 'sync' mode).
     *
     * @param isFrameComplete Marks a transmission frame as complete. Should always
     * be `true` in 'simple' mode.
     */
    send(data: Uint8Array | undefined, isFrameComplete: boolean): void;

    /**
     * Switch between 'simple' and 'sync' mode. Sync mode consumes more CPU and should
     * only be used to connect two emulator instances via IrDA.
     *
     * @param modeSync Set `true` for 'sync' mode.
     */
    setModeSync(modeSync: boolean): void;

    /**
     * Query mode.
     */
    getModeSync(): boolean;

    /**
     * This is fired by the emulator for data or synchronization events. In 'sync' mode,
     * **every** such event needs to be sent to the remote instance.
     */
    receiveEvent: CloudpilotEvent<ReceivePayload>;
}
