/** @jsx jsx */

import 'react';

import { Fragment, FunctionComponent } from 'react';

import Canvas from './Canvas';
import EmulationContext from './EmulationContext';
import EmulationService from './service/EmulationService';
import { jsx } from '@emotion/core';
import { render } from 'react-dom';

const App: FunctionComponent = () => (
    <Fragment>
        <h1>Cloudpilot!</h1>

        <Canvas />
    </Fragment>
);

async function main() {
    const emulationService = await EmulationService.create();
    const rom = new Uint8Array(await (await fetch('./test.rom')).arrayBuffer());

    await emulationService.initialize(rom, 'PalmV');
    emulationService.start();

    render(
        <EmulationContext.Provider value={emulationService}>
            <App />
        </EmulationContext.Provider>,
        document.getElementById('react-mountpoint')
    );
}

main();
