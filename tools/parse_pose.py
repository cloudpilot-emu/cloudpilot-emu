#!/usr/bin/env python3

import zlib
from struct import pack_into, unpack, unpack_from

deviceId = None
ram = None
romName = None
savestate = None


def decompressRAM(chunk: bytes) -> bytes:
    if len(chunk) < 4:
        raise RuntimeError('bad RAM chunk')

    uncompressedSize = unpack('>I', chunk[:4])[0]
    uncompressedImage = zlib.decompress(chunk[4:], -15)

    if uncompressedSize != len(uncompressedImage):
        raise RuntimeError(
            f'bad RAM image: expected {uncompressedSize} bytes, got {len(uncompressedImage)} bytes')

    print(f"got {uncompressedSize} bytes of RAM")

    return uncompressedImage


with open('iiic.psf', 'rb') as reader:
    savestate = reader.read()

offset = 0
while offset < len(savestate):
    if len(savestate) - offset < 5:
        raise RuntimeError('invalid savestate file')

    tag = savestate[offset:offset + 4]
    offset += 4

    chunkSize = unpack_from('>I', savestate, offset)[0]
    offset += 4

    if offset + chunkSize > len(savestate):
        raise RuntimeError('invalid savestate file')

    chunk = savestate[offset:offset + chunkSize]
    offset += chunkSize

    if tag == b'DStr':
        deviceId = chunk
        print(f'device ID: {deviceId.decode("utf-8")}')

    elif tag == b'ROMn':
        romName = chunk.decode("utf-8")
        print(f'ROM file: {romName}')

    elif tag == b'ROMp' or tag == b'ROMu':
        print(f'Path to ROM file: {chunk.decode("utf-8")}')

    elif tag == b'zram':
        ram = decompressRAM(chunk)

if romName is None or ram is None or deviceId is None:
    raise RuntimeError("incomplete image")

rom = None
with open(romName, "rb") as reader:
    rom = reader.read()

ramSwapped = bytearray(len(ram))

words = unpack(f'<{len(ram) >> 1}H', ram)
pack_into(f'>{len(ramSwapped) >> 1}H', ramSwapped, 0, *words)

image = bytearray(28 + len(deviceId) + len(rom) + len(ramSwapped))

pack_into('<IIIIIII', image, 0, 0x20150103, 0x80000001,
          len(deviceId), 0, len(rom), len(ramSwapped), 0)

offset = 28

image[offset:offset+len(deviceId)] = deviceId
offset += len(deviceId)

image[offset:offset+len(rom)] = rom
offset += len(rom)

image[offset:offset+len(ramSwapped)] = ramSwapped

with open("image_from_pose.img", "wb") as writer:
    writer.write(image)
