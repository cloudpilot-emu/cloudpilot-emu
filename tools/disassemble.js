#!/usr/bin/env node

const fs = require('fs');
const { execSync, exec } = require('child_process');
const traps = require('./traps.js');

let iAnonymousFunction = 0;
const functions = new Map();

function anonymousFunction() {
    return `anonymous_function_${iAnonymousFunction++}`;
}

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
    const branchRegex =
        /^\s*\w+:\s+(?:(?:\w{4})|(?: {4})) (?:(?:\w{4})|(?: {4})) (?:(?:\w{4})|(?: {4})) \s*b\w+\s+0x([0-9a-f]+)/;

    for (const [, { lines }] of functions) {
        let iLabel = 0;

        for (const [address, line] of lines) {
            const match = line.disassembly.match(branchRegex);
            if (!match) continue;

            const target = parseInt(match[1], 16);
            if (!lines.has(target)) continue;

            const targetLine = lines.get(target);
            if (targetLine.label === undefined) {
                targetLine.label = true;
            }
        }

        for (const [, line] of lines) {
            if (line.label) line.label = `label_${iLabel++}`;
        }

        for (const [address, line] of lines) {
            const match = line.disassembly.match(branchRegex);
            if (!match) continue;

            const target = parseInt(match[1], 16);
            if (!lines.has(target)) continue;

            const targetLine = lines.get(target);

            line.meta.push(`${target >= address ? '↓' : '↑'} ${targetLine.label}`);
        }
    }
}

function collectParameters() {
    for (const [, entry] of functions) {
        entry.parameters = new Set();

        for ([address, line] of entry.lines) {
            const match = line.disassembly.match(/%fp@\((\d+)\)/);
            if (!match) continue;

            entry.parameters.add(parseInt(match[1], 10));
        }
    }
}

function collectLocals() {
    for (const [, entry] of functions) {
        entry.locals = new Set();

        for ([address, line] of entry.lines) {
            const match = line.disassembly.match(/%fp@\(-(\d+)\)/);
            if (!match) continue;

            entry.locals.add(parseInt(match[1], 10));
        }
    }
}

function disassembleAdditionalFunctions(buffer) {
    const processed = new Set();
    let nIdentifiedFunctions;

    do {
        nIdentifiedFunctions = functions.size;

        for (const [, { lines }] of functions)
            for ([address, line] of lines) {
                const match = line.disassembly.match(/jsr %pc@\(0x([0-9a-f]+)\)/);
                if (!match) continue;

                const target = parseInt(match[1], 16);

                if (!functions.has(target) && !processed.has(target) && target < buffer.length) {
                    disassembleFunctionAt(buffer, target);
                    processed.add(target);
                }
            }
    } while (functions.size > nIdentifiedFunctions);
}

function disassembleFunctionAt(buffer, entry) {
    if (entry % 2 !== 0) throw new Error(`call address 0x${entry.toString(16)} is not aligned`);
    const buffer16 = new Uint16Array(buffer.buffer, buffer.byteOffset, buffer.length >>> 1);

    for (let i = entry >> 1; i < buffer16.length; i++) {
        if (buffer16[i] !== 0x754e) continue;

        disassemble(entry, 2 * i, readFunctionName(buffer, 2 * i) || anonymousFunction());
        return;
    }
}

function readFunctionName(buffer, rtsAt) {
    let name = '';

    for (let i = rtsAt + 3; i < buffer.length; i++) {
        const char = buffer[i];

        if (char > 127) return undefined;
        if (char === 0) return name;
        if (i % 2 === 0 && char === 0x4e && buffer[i + 1] === 0x56) return undefined;

        name += String.fromCharCode(char);

        if (name.length >= 30) return name;
    }
}

let link = 0;
let mode = 'search-link';

const file = process.argv[2];
const binary = fs.readFileSync(file);
const binary16 = new Uint16Array(binary.buffer, binary.byteOffset, binary.length >>> 1);

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
                disassemble(link, 2 * i, readFunctionName(binary, 2 * i) || anonymousFunction());

                mode = 'search-link';
            }

            if (opcode === 0x564e) {
                link = 2 * i;
            }

            break;

        default:
            throw new Error('cannot happen');
    }
}

disassembleAdditionalFunctions(binary);
assignLabels();
identifyCalls();
resolveTraps();
collectParameters();
collectLocals();

for (const entry of Array.from(functions.keys()).sort((x, y) => x - y)) {
    const { start, stop, name, lines, parameters, locals } = functions.get(entry);

    const addressWidth = Math.max(...lines.keys()).toString(16).length;

    console.log(`# ${name}: 0x${start.toString(16).padStart(4, '0')} - 0x${stop.toString(16).padStart(4, '0')}`);
    console.log();

    if (parameters.size > 0) {
        console.log('Parameters:');
        console.log();
        for (const parameter of Array.from(parameters.values()).sort((x, y) => x - y)) {
            console.log(`   * \`%fp@(${parameter})\``.padEnd(20) + ': ???');
        }
        console.log();
    }

    if (locals.size > 0) {
        console.log('Locals:');
        console.log();
        for (const local of Array.from(locals.values()).sort((x, y) => x - y)) {
            console.log(`   * \`%fp@(-${local})\``.padEnd(20) + ': ???');
        }
        console.log();
    }

    console.log('```');

    for ([address, { disassembly, meta, label }] of lines) {
        if (label) {
            console.log();
            console.log(`  ${label}:`);
        }

        console.log(
            `    ${disassembly}`
                .replace('\t', '  ')
                .replace(/^\w+:\s+/, `${address.toString(16).padStart(addressWidth, '0')}: `)
                .padEnd(65) + (meta.length > 0 ? `; ${meta.join(' ')}` : '')
        );
    }

    console.log('```');
    console.log();
}
