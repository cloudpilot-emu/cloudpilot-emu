#!/usr/bin/env node

const fs = require('fs');
const { execSync, exec } = require('child_process');
const traps = require('./traps.js');

const file = process.argv[2];
const binary = fs.readFileSync(file);
const binary16 = new Uint16Array(binary.buffer);

const functions = new Map();

function disassemble(start, stop, name) {
    const lines = execSync(
        `m68k-palmos-objdump -b binary -m m68k:68000 -D --start-address=${start} --stop-address=${stop + 2} "${file}"`
    )
        .toString('utf-8')
        .replace(/^[\s\S]+>:/m, '')
        .split('\n')
        .filter((x) => !!x)
        .map((line) => {
            const match = line.trim().match(/^([0-9a-f]+):.*/);

            if (!match) {
                throw new Error(`malformed disassembly line '${line}'`);
            }

            return match ? [parseInt(match[1], 16), { disassembly: match[0], meta: [] }] : undefined;
        })
        .filter((x) => !!x);

    functions.set(start, { start, stop, name, lines: new Map(lines) });
}

function identifyCalls() {
    for (const [, { lines }] of functions)
        for ([address, line] of lines) {
            const match = line.disassembly.match(/%pc@\(0x([0-9a-f]+)\)/);
            if (!match) continue;

            const target = parseInt(match[1], 16);
            line.meta.push(functions.get(target)?.name || `??? 0x${match[1]}`);
        }
}

function resolveTraps() {
    for (const [, { lines }] of functions)
        for ([address, line] of lines) {
            const match = line.disassembly.match(/^\s*\w+:\s+(a[0-9a-f]{3})\s+\d+\s*$/);
            if (!match) continue;

            const trap = parseInt(match[1], 16);
            line.meta.push(traps.get(trap) || `??? 0x${match[1]}`);
        }
}

function assignLabels() {
    for (const [, { lines }] of functions) {
        let iLabel = 0;

        for ([address, line] of lines) {
            const match = line.disassembly.match(
                /^\s*\w+:\s+(?:(?:\w{4})|(?: {4}) ) (?:(?:\w{4})|(?: {4}) )(?:(?:\w{4})|(?: {4}) )\s*b\w+\s+0x([0-9a-f]+)/
            );
            if (!match) continue;

            const target = parseInt(match[1], 16);
            if (!lines.has(target)) continue;

            const targetLine = lines.get(target);
            if (targetLine.label === undefined) {
                targetLine.label = `lbl_${iLabel++}`;
                targetLine.meta.unshift(`:${targetLine.label}`);
            }

            line.meta.push(`-> ${targetLine.label}`);
        }
    }
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

assignLabels();
identifyCalls();
resolveTraps();

for (const [, { start, stop, name, lines }] of functions) {
    console.log(`# ${name}: 0x${start.toString(16).padStart(4, '0')} - 0x${stop.toString(16).padStart(4, '0')}`);
    console.log();
    console.log('```');

    const addressWidth = Math.max(...lines.keys()).toString(16).length;

    for ([address, { disassembly, meta }] of lines) {
        console.log(
            disassembly
                .replace('\t', '  ')
                .replace(/^\w+:\s+/, `${address.toString(16).padStart(addressWidth, '0')}: `)
                .padEnd(65) + (meta.length > 0 ? `; ${meta.join(' ')}` : '')
        );
    }
    console.log();
}
