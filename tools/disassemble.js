#!/usr/bin/env node

const fs = require('fs');
const { execSync, exec } = require('child_process');

const file = process.argv[2];
const binary = fs.readFileSync(file);
const binary16 = new Uint16Array(binary.buffer);

function disassemble(start, stop, name) {
    console.log(`# ${name}: 0x${start.toString(16).padStart(4, '0')} - 0x${stop.toString(16).padStart(4, '0')}`);
    console.log();
    console.log();
    console.log('```');
    console.log(
        execSync(
            `m68k-palmos-objdump -b binary -m m68k -D --start-address=${start} --stop-address=${stop + 2} "${file}"`
        )
            .toString('utf-8')
            .replace(/^[\s\S]+>:/m, '')
    );
    console.log('```');
    console.log();
}

let link = 0;
let rts = 0;
let mode = 'search-link';
let name = '';

for (let i = 0; i < binary16.length; i++) {
    const opcode = binary16[i];

    switch (mode) {
        case 'search-link':
            if (opcode === 0x564e) {
                link = 2 * i;
                mode = 'search-rts';
            }

            break;

        case 'search-rts':
            if (opcode === 0x754e && link > 0) {
                rts = 2 * i;
                mode = 'scan-name';
                name = '';
            }

            if (opcode === 0x564e) {
                link = 2 * i;
            }

            break;

        case 'scan-name': {
            const fst = opcode & 0xff;
            const snd = opcode >> 8;

            if (opcode === 0x564e) {
                disassemble(link, rts, '[unknown]');
                link = 2 * i;
                mode = 'search-rts';
            } else if (fst === 0) {
                disassemble(link, rts, name.substring(1));
                mode = 'search-link';
            } else if (snd === 0) {
                name += String.fromCharCode(fst);
                disassemble(link, rts, name.substring(1));
                mode = 'search-link';
            } else {
                name = name + String.fromCharCode(fst) + String.fromCharCode(snd);
            }

            break;
        }

        default:
            throw new Error('cannot happen');
    }
}
