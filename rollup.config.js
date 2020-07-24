/* eslint-env node */

'use strict';

import * as fs from 'fs';
import * as path from 'path';

import alias from '@rollup/plugin-alias';
import commonjs from '@rollup/plugin-commonjs';
import { compile } from 'handlebars';
import copy from 'rollup-plugin-copy';
import html from '@rollup/plugin-html';
import replace from '@rollup/plugin-replace';
import resolve from '@rollup/plugin-node-resolve';
import typescript from '@rollup/plugin-typescript';

const DEV = !!process.env['DEV'];

export default [
    {
        input: 'web/src/main.tsx',
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
            html({ title: 'Cloudpilot', template }),
            replace({
                'process.env.NODE_ENV': JSON.stringify(DEV ? 'development' : 'production'),
            }),
        ],
    },
];

function template(scope) {
    const source = fs.readFileSync(path.join(__dirname, 'web', 'site', 'index.handlebars'), { encoding: 'utf-8' });
    const tpl = compile(source);

    return tpl(scope);
}
