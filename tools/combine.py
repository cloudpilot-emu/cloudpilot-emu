#!/usr/bin/env python3

from struct import pack_into, unpack

romImage = None
ramImageSwapped = None

with open("ram_from_pose.", "rb") as reader:
    romImage = reader.read()

with open("ram_m515.img", "rb") as reader:
    ramImageSwapped = reader.read()

ramImage = bytearray(16 * 1024 * 1024)


offset = 16*1024*1024 - len(ramImageSwapped)
words = unpack(f'<{len(ramImageSwapped) >> 1}H', ramImageSwapped)
pack_into(f'>{len(ramImageSwapped) >> 1}H', ramImage, offset, *words)

deviceId = b'PalmM515'

image = bytearray(28 + len(deviceId) + len(romImage) + len(ramImage))

pack_into('<IIIIIII', image, 0, 0x20150103, 0x80000001,
          len(deviceId), 0, len(romImage), len(ramImage), 0)

offset = 28

image[offset:offset+len(deviceId)] = deviceId
offset += len(deviceId)

image[offset:offset+len(romImage)] = romImage
offset += len(romImage)

image[offset:offset+len(ramImage)] = ramImage

with open("image.img", "wb") as writer:
    writer.write(image)
