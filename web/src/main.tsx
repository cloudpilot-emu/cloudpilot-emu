import EmulationService from './service/EmulationService';
import { FunctionComponent } from 'react';
import React from 'react';
import { render } from 'react-dom';

const App: FunctionComponent = () => <div>Hello world!</div>;

async function main() {
    const emulationService = new EmulationService();
    const rom = new Uint8Array(await (await fetch('./test.rom')).arrayBuffer());

    await emulationService.initialize(rom, 'PalmV');

    render(<App />, document.getElementById('react-mountpoint'));
}

main();
