/** @jsx jsx */

import 'react';

import { FunctionComponent, useContext, useEffect, useRef } from 'react';

import EmulationContext from './EmulationContext';
import { jsx } from '@emotion/core';

const Canvas: FunctionComponent = () => {
    const emulationService = useContext(EmulationContext);
    const canvasRef = useRef<HTMLCanvasElement>(null);

    if (!emulationService) {
        throw new Error('no emulation context');
    }

    useEffect(() => {
        if (!canvasRef.current) {
            throw new Error('no canvas element');
        }

        emulationService.setCanvas(canvasRef.current);
    }, []);

    return (
        <canvas
            width="160"
            height="220"
            css={{ width: 480, height: 660, imageRendering: 'pixelated' }}
            ref={canvasRef}
        ></canvas>
    );
};

export default Canvas;
