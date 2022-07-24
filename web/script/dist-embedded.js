const shelljs = require('shelljs');
const path = require('path');
const fs = require('fs');

const OUT_DIR = path.normalize(path.join(__dirname, '..', 'dist-embedded'));
const BUILD_SRC = path.normalize(path.join(__dirname, '..', 'build-embedded'));
const BUILD_DEST = path.normalize(path.join(OUT_DIR, 'examples'));
const TYPEDOC_SRC = path.normalize(path.join(__dirname, '..', 'docs-embedded'));
const TYPEDOC_DEST = path.normalize(path.join(OUT_DIR, 'reference'));
const README = path.normalize(path.join(__dirname, '..', 'embedded', 'doc', 'README.md'));

if (fs.existsSync(OUT_DIR)) shelljs.rm('-r', OUT_DIR);

shelljs.mkdir(OUT_DIR);
shelljs.cp('-R', BUILD_SRC, BUILD_DEST);
shelljs.cp('-R', TYPEDOC_SRC, TYPEDOC_DEST);
shelljs.cp(README, OUT_DIR);
