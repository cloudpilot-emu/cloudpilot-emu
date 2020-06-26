'use strict';

import alias from '@rollup/plugin-alias';
import commonjs from '@rollup/plugin-commonjs';
import copy from 'rollup-plugin-copy';
import html from '@rollup/plugin-html';
import resolve from '@rollup/plugin-node-resolve';
import typescript from '@rollup/plugin-typescript';
export default [
    {
        input: 'web/src/main.ts',
        output: {
            file: 'dist/main.js',
            format: 'iife',
            sourcemap: 'true',
            name: '$cloudpilot',
        },
        plugins: [
            alias({
                resolve: ['.ts', '.tsx'],
                entries: [{ find: 'native', replacement: '../../src' }],
            }),
            resolve({ browser: true }),
            commonjs(),
            typescript({ noEmitOnError: false }),
            copy({ targets: [{ src: 'src/cloudpilot_web.wasm', dest: 'dist' }] }),
            html({ title: 'Cloudpilot' }),
        ],
    },
];
