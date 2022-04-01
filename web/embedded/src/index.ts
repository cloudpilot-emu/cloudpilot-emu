import { Cloudpilot } from '@common/Cloudpilot';

export const VERSION = 'dev';

async function main() {
    (window as any).cloudpilotInstance = await Cloudpilot.create();
}

main().catch((e) => console.log(e));
