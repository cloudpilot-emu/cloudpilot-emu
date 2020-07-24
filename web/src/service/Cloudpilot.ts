import createModule, { CloudpilotNative, Module, VoidPtr } from 'native';

export interface RomInfo {
    cardVersion: number;
    cardName: string;
    romVersion: number;
    romVersionString: string;
}

export interface Frame {
    bpp: number;
    lineWidth: number;
    lines: number;
    margin: number;
    bytesPerLine: number;

    buffer: Int8Array;
}

class Cloudpilot {
    private constructor(private module: Module) {
        this.cloudpilot = new module.Cloudpilot();
    }

    static async create(): Promise<Cloudpilot> {
        return new Cloudpilot(
            await createModule({
                print: (x: string) => console.log(x),
                printErr: (x: string) => console.error(x),
            })
        );
    }

    getRomInfo(rom: Uint8Array): RomInfo | undefined {
        const buffer = this.copyIn(rom);

        const romInfoNative = new this.module.RomInfo();
        let romInfo: RomInfo | undefined;

        if (this.cloudpilot.GetRomInfo(buffer, rom.length, romInfoNative)) {
            romInfo = {
                cardVersion: romInfoNative.CardVersion(),
                cardName: romInfoNative.CardName(),
                romVersion: romInfoNative.RomVersion(),
                romVersionString: romInfoNative.RomVersionString(),
            };
        }

        this.cloudpilot.Free(buffer);
        this.module.destroy(romInfoNative);

        return romInfo;
    }

    initializeSession(rom: Uint8Array, deviceType: string): boolean {
        const buffer = this.copyIn(rom);

        const result = this.cloudpilot.InitializeSession(buffer, rom.length, deviceType);

        this.cloudpilot.Free(buffer);

        return result;
    }

    destroy(): void {
        this.module.destroy(this.cloudpilot);
    }

    cyclesPerSecond(): number {
        return this.cloudpilot.GetCyclesPerSecond();
    }

    runEmulation(cycles: number): number {
        return this.cloudpilot.RunEmulation(cycles);
    }

    getFrame(): Frame | null {
        const nativeFrame = this.cloudpilot.CopyFrame();

        const bufferPtr = this.module.getPointer(nativeFrame.GetBuffer());

        return nativeFrame.lineWidth === 160 && nativeFrame.lines === 160
            ? {
                  bpp: nativeFrame.bpp,
                  bytesPerLine: nativeFrame.bytesPerLine,
                  lines: nativeFrame.lines,
                  lineWidth: nativeFrame.lineWidth,
                  margin: nativeFrame.margin,
                  buffer: this.module.HEAP8.subarray(bufferPtr, bufferPtr + nativeFrame.GetBufferSize()),
              }
            : null;
    }

    isScreenDirty(): boolean {
        return this.cloudpilot.IsScreenDirty();
    }

    markScreenClean(): void {
        this.cloudpilot.MarkScreenClean();
    }

    private copyIn(data: Uint8Array): VoidPtr {
        const buffer = this.cloudpilot.Malloc(data.length);
        const bufferPtr = this.module.getPointer(buffer);

        this.module.HEAP8.subarray(bufferPtr, bufferPtr + data.length).set(data);

        return buffer;
    }

    private readonly cloudpilot: CloudpilotNative;
}

export default Cloudpilot;
