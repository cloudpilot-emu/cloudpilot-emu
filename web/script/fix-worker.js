const fs = require('fs');

const worker = fs.readFileSync(process.argv[2], 'utf-8');

const patched = worker.replaceAll(
    /cache\.(delete|match|put)\((.*?)\)(\s*[;.=!])/g,
    'cache.$1($2).catch(e => console.log(e))$3',
);

fs.writeFileSync(process.argv[2], patched, 'utf-8');
