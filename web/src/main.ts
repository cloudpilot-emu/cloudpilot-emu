import createModule, { CloudpilotNative, VoidPtr } from 'native';

async function main() {
    const module = await createModule({
        print: (x: string) => console.log(x),
        printErr: (x: string) => console.log(x),
    });

    const cloudpilot: CloudpilotNative = new module.Cloudpilot();

    const buffer: VoidPtr = cloudpilot.malloc(1024);

    console.log(buffer);
    console.log(module.getPointer(buffer));

    cloudpilot.free(buffer);
}

main();
