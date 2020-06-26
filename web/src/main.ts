import Cloudpilot from './Cloudpilot';

async function main() {
    const cloudpilot = await Cloudpilot.create();
    const rom = new Uint8Array(await (await fetch('./test.rom')).arrayBuffer());
    const romInfo = cloudpilot.getRomInfo(rom);

    console.log(romInfo);
}

main();
