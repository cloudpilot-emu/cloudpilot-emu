import * as assert from 'assert';

import { crc32 } from '../helper/crc';

describe('crc', () => {
    describe('crc32', () => {
        it('calculates crc32 correctly', () => {
            const fixture = new Uint8Array('123456789'.split('').map((x) => x.charCodeAt(0)));

            assert.equal(crc32(fixture), 0xcbf43926 | 0);
        });
    });
});
