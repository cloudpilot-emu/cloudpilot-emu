const shelljs = require('shelljs');
const path = require('path');
const fs = require('fs');
const packageJson = require('../package.json');

const PACKAGE_JSON_TEMPLATE = {
    name: 'cloudpilot-emu-embedded',
    author: 'Christian Speckner <cnspeckn@googlemail.com>',
    license: 'GPL-3.0-or-later',
    homepage: 'https://cloudpilot-emu.github.io/',
    description: 'Embedded version of the CloudpilotEmu PalmOS emulator',
    keywords: ['cloudpilot', 'cloudpilotemu', 'palmos', 'palm', 'emulator'],
    files: ['README.md', 'LICENSE', 'dist/**/*'],
    main: 'dist/cloudpilot-emu.js',
    types: 'dist/cloudpilot-emu.d.ts',
    dependencies: {},
    scripts: {},
};

const OUT_DIR = path.normalize(path.join(__dirname, '..', 'dist-embedded'));
const BUILD_SRC = path.normalize(path.join(__dirname, '..', 'build-embedded'));
const BUILD_DEST = path.normalize(path.join(OUT_DIR, 'examples'));
const TYPEDOC_SRC = path.normalize(path.join(__dirname, '..', 'docs-embedded'));
const TYPEDOC_DEST = path.normalize(path.join(OUT_DIR, 'reference'));
const README = path.normalize(path.join(__dirname, '..', 'embedded', 'doc', 'README.md'));

const NPM_PACKAGE_DIR = path.normalize(path.join(__dirname, '..', 'npm-embedded'));
const NPM_DIST = ['cloudpilot-emu.js', 'cloudpilot-emu.d.ts', 'cloudpilot-emu.js.map', 'cloudpilot_web.wasm'].map((x) =>
    path.normalize(path.join(BUILD_SRC, x)),
);
const NPM_DIST_TARGET = path.normalize(path.join(NPM_PACKAGE_DIR, 'dist'));

const LICENSE = path.normalize(path.join(__dirname, '..', '..', 'LICENSE'));
const README_NPM_SOURCE = path.normalize(path.join(__dirname, '..', 'embedded', 'doc', 'README.npm.md'));
const README_NPM_DEST = path.normalize(path.join(NPM_PACKAGE_DIR, 'README.md'));
const PACKAGE_JSON_DEST = path.normalize(path.join(NPM_PACKAGE_DIR, 'package.json'));

if (fs.existsSync(OUT_DIR)) shelljs.rm('-r', OUT_DIR);

shelljs.mkdir(OUT_DIR);
shelljs.cp('-R', BUILD_SRC, BUILD_DEST);
shelljs.cp('-R', TYPEDOC_SRC, TYPEDOC_DEST);
shelljs.cp(README, OUT_DIR);

if (fs.existsSync(NPM_DIST_TARGET)) shelljs.rm('-r', NPM_DIST_TARGET);

shelljs.mkdir('-p', NPM_DIST_TARGET);
NPM_DIST.forEach((x) => shelljs.cp(x, NPM_DIST_TARGET));
shelljs.cp(README_NPM_SOURCE, README_NPM_DEST);
shelljs.cp(LICENSE, NPM_PACKAGE_DIR);
fs.writeFileSync(
    PACKAGE_JSON_DEST,
    JSON.stringify({ ...PACKAGE_JSON_TEMPLATE, version: packageJson.version }, undefined, '  '),
);
