import { SerialTransport } from '@common/bridge/Cloudpilot';
import { ReceivePayload, SerialPort } from '@common/serial/SerialPort';
import { Event } from 'microevent.ts';

const MAX_IRDA_FRAME_BUFFER = 1024;

export class SerialPortImpl implements SerialPort {
    bind(transport: SerialTransport): void {
        if (this.transport === transport) return;

        this.unbind();

        transport.frameCompleteEvent.addHandler(this.onFrameComplete);
        transport.SetModeSync(this.isSync);

        this.transport = transport;
    }

    unbind(): void {
        if (!this.transport) return;

        this.transport.frameCompleteEvent.removeHandler(this.onFrameComplete);
        this.transport = undefined;
    }

    send(data: Uint8Array | undefined, isFrameComplete: boolean): void {
        if (!this.transport) return;

        if (!this.transport.IsOpen() && this.transport.GetModeSync()) {
            this.receiveEvent.dispatch({ data: undefined, isFrameComplete: true });
            return;
        }

        this.transport.Send(data, isFrameComplete);
        setTimeout(() => this.dispatchEvent.dispatch(), 0);
    }

    setModeSync(modeSync: boolean): void {
        this.isSync = modeSync;
        this.transport?.SetModeSync(modeSync);
    }

    getModeSync(): boolean {
        return this.transport?.GetModeSync() ?? false;
    }

    dispatch(): void {
        if (!this.transport) return;

        const bytesPending = this.transport.RxBytesPending();
        if (bytesPending === 0) return;
        if (this.transport.GetModeSync() && bytesPending < MAX_IRDA_FRAME_BUFFER) return;

        this.receiveEvent.dispatch({
            data: this.transport.Receive(),
            isFrameComplete: this.transport.IsFrameComplete(),
        });
    }

    private onFrameComplete = () => {
        if (!this.transport) return;

        this.receiveEvent.dispatch({
            data: this.transport.Receive(),
            isFrameComplete: this.transport.IsFrameComplete(),
        });
    };

    receiveEvent = new Event<ReceivePayload>();
    dispatchEvent = new Event<void>();

    private transport: SerialTransport | undefined;
    private isSync = false;
}
