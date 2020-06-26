'use strict';

import resolve from '@rollup/plugin-node-resolve';
import typescript from '@rollup/plugin-typescript';
import commonjs from '@rollup/plugin-commonjs';
import html from '@rollup/plugin-html';
import { eslint } from 'rollup-plugin-eslint';

export default [
    {
        input: 'web/src/index.ts',
        output: {
            file: 'dist/main.js',
            format: 'iife',
            sourcemap: 'true',
            name: '$cloudpilot',
        },
        plugins: [
            eslint({ include: 'web/src/**' }),
            html({ title: 'Cloudpilot' }),
            resolve({ browser: true }),
            typescript({ noEmitOnError: false }),
            commonjs(),
        ],
    },
];
