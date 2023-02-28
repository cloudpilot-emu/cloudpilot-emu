const fs = require('fs');

const data = fs.readFileSync(process.argv[2]);
const compressed = [];

function encodeChar(value, repeat) {
    if (repeat === 1 && value !== 0xff) {
        compressed.push(value);
    } else {
        compressed.push(0xff, value);

        for (let j = 0; j < 4; j++) {
            compressed.push(repeat > 0x7f ? ((repeat & 0x7f) | 0x80) : repeat);
            repeat >>>= 7;

            if (repeat === 0) break;
        }
    }
}

let last = data[0];
let repeat = 1;
for (let i = 1; i < data.length; i++) {
    const value = data[i];

    if (value === last) {
        repeat++;
        continue;
    }

    encodeChar(last, repeat);

    repeat = 1;
    last = value;
}

encodeChar(last, repeat);

console.log(`{ ${compressed.map(x => '0x' + x.toString(16).padStart(2, 0)).join(', ')} }`);

