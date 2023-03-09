#!/usr/bin/env python3

import argparse
from struct import calcsize, unpack_from


def transformZeroTerminatedString(str: bytes) -> str:
    terminator = str.find(0)

    return str[:terminator if terminator >= 0 else len(str)].decode('ascii')


class Record:
    _format: str = ""

    def size(self) -> int:
        return calcsize(self._format)


class PrcHeader(Record):
    name: str
    attributes: int
    version: int
    creationDate: int
    modificationDate: int
    lastBackupDate: int
    modificationNumber: int
    appInfoID: int
    sortInfoID: int
    type: bytes
    creator: bytes
    uniqueIDSeed: int

    _format: str = ">32s2H6I4s4sI"

    def __init__(self, buffer: bytes, offset: int):
        (encodedName,
         self.attributes,
         self.version,
         self.creationDate,
         self.modificationDate,
         self.lastBackupDate,
         self.appInfoID,
         self.appInfoID,
         self.sortInfoID,
         self.type,
         self.creator,
         self.uniqueIDSeed) = unpack_from(self._format, buffer, offset)

        self.name = transformZeroTerminatedString(encodedName)


class RecordList(Record):
    nextRecordListID: int
    numRecords: int

    _format: str = ">IH"

    def __init__(self, buffer: bytes, offset: int):
        (self.nextRecordListID, self.numRecords) = unpack_from(
            self._format, buffer, offset)


class ResourceEntry(Record):
    type: bytes
    id: int
    localChunkID: int

    _format: str = ">4sHI"

    def __init__(self, buffer: bytes, offset: int):
        (self.type, self.id, self.localChunkID) = unpack_from(
            self._format, buffer, offset)


def splitResources(options):
    try:
        with open(options.target, "rb") as reader:
            prcfile: bytes = reader.read()

    except Exception:
        print(f'unable to read {options.target}')
        return

    try:
        offset = 0

        header = PrcHeader(prcfile, offset)
        offset += header.size()

        recordList = RecordList(prcfile, offset)
        offset += recordList.size()

        if recordList.nextRecordListID != 0:
            raise RuntimeError("unable to parse multi-segmented DB")

        resourceEntries: list[ResourceEntry] = []
        for i in range(0, recordList.numRecords):
            resourceEntry = ResourceEntry(prcfile, offset)
            offset += resourceEntry.size()

            resourceEntries.append(resourceEntry)

    except Exception as ex:
        print(f'unable to parse prc: {ex}')
        return

    print(f'name: {header.name}')
    print(f'type: {header.type.decode("ascii")}')
    print(f'creator: {header.creator.decode("ascii")}')
    print(f'writing {recordList.numRecords} resource entries')

    for i, resourceEntry in enumerate(resourceEntries):
        filename = f'{options.prefix}{resourceEntry.type.decode("ascii")}_{resourceEntry.id}'
        print(f'writing {filename}')

        start = resourceEntry.localChunkID
        end = resourceEntries[i +
                              1].localChunkID if i < len(resourceEntries) - 1 else len(prcfile)

        if end < start or start >= len(prcfile) or end > len(prcfile):
            print("   ...bad chunk!")
        else:
            with open(filename, 'bw') as writer:
                writer.write(prcfile[start:end])


parser = argparse.ArgumentParser(description="Tools for handling prc files")
subparsers = parser.add_subparsers(help="subcommand", dest="subcommand")

parserSplit = subparsers.add_parser(
    "split-resources", help="split into individual resources", description="Split the .prc into its individual resources")
parserSplit.add_argument("target", metavar="<target>",
                         type=str, help="target file")
parserSplit.add_argument("prefix", metavar="<prefix>",
                         type=str, help="target file")

options = parser.parse_args()

if options.subcommand == "split-resources":
    splitResources(options)

else:
    parser.print_help()
