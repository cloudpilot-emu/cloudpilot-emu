import createModule, { CloudpilotNative } from 'native';

async function main() {
    const module = await createModule({
        print: (x: string) => console.log(x),
        printErr: (x: string) => console.log(x),
    });

    const cloudpilot: CloudpilotNative = new module.Cloudpilot();
    console.log(cloudpilot.hello());
}

main();
