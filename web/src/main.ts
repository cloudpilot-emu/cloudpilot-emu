import createModule from 'native';

async function main() {
    const module = await createModule({
        print: (x: string) => console.log(x),
        printErr: (x: string) => console.log(x),
        noInitialRun: true,
    });

    module.ccall('main', 'number', [], []);
}

main();
