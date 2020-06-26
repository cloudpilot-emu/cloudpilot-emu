import createModule, { CloudpilotNative, Module, VoidPtr } from 'native';

export interface RomInfo {
    cardVersion: number;
    cardName: string;
    romVersion: number;
    romVersionString: string;
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

    destroy(): void {
        this.module.destroy(this.cloudpilot);
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
