#!/usr/bin/env python3

import argparse
from struct import calcsize, pack_into, unpack_from

dmHdrAttrResDB = 0x0001
dmRecAttrDelete = 0x80


def transformZeroTerminatedString(str: bytes) -> str:
    terminator = str.find(0)

    return str[:terminator if terminator >= 0 else len(str)].decode('ascii')


class Record:
    _format: str = ""

    def size(self) -> int:
        return calcsize(self._format)


class Header(Record):
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
    offset: int

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

        self.offset = offset

        self.name = transformZeroTerminatedString(encodedName)


class RecordList(Record):
    nextRecordListID: int
    numRecords: int
    offset: int

    _format: str = ">IH"

    def __init__(self, buffer: bytes, offset: int):
        (self.nextRecordListID, self.numRecords) = unpack_from(
            self._format, buffer, offset)

        self.offset = offset


class ResourceEntry(Record):
    type: bytes
    id: int
    localChunkID: int
    offset: int

    _format: str = ">4sHI"

    def __init__(self, buffer: bytes, offset: int):
        (self.type, self.id, self.localChunkID) = unpack_from(
            self._format, buffer, offset)

        self.offset = offset


class RecordEntry(Record):
    localChunkID: int
    attributes: int
    unique_id: int
    offset: int

    _format: str = ">IB3s"

    def __init__(self, buffer: bytes, offset: int):
        encoded_id: bytearray

        (self.localChunkID, self.attributes, encoded_id) = unpack_from(
            self._format, buffer, offset)

        self.unique_id = encoded_id[2] | (
            encoded_id[1] << 8) | (encoded_id[0] << 16)

        self.offset = offset


def splitResources(options):
    try:
        with open(options.target, "rb") as reader:
            prcfile: bytes = reader.read()

    except Exception:
        print(f'unable to read {options.target}')
        return

    try:
        offset = 0

        header = Header(prcfile, offset)
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

    if (header.attributes & dmHdrAttrResDB == 0):
        print("not a resource DB")
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


def fix_pdb(options):
    try:
        with open(options.source, "rb") as reader:
            pdbfile_in = reader.read()

    except Exception:
        print(f'unable to read {options.source}')
        return

    pdbfile_out = bytearray(len(pdbfile_in))
    pdbfile_out[:] = pdbfile_in

    try:
        header = Header(pdbfile_in, 0)
        offset = header.size()

        if (header.attributes & dmHdrAttrResDB != 0):
            print("not a record DB")
            return

        recordList = RecordList(pdbfile_in, offset)
        offset += recordList.size()

    except Exception as ex:
        print(f"faile to parse {options.source}: {ex}")

    print(f"DB name: {header.name}")
    print(f"found {recordList.numRecords} records")

    record_entries: list[RecordEntry] = []

    for i in range(0, recordList.numRecords):
        entry = RecordEntry(pdbfile_in, offset)
        offset += entry.size()

        record_entries.append(entry)

    deleted_records = 0

    for i, entry in enumerate(record_entries):
        if (entry.attributes & dmRecAttrDelete == 0):
            continue

        deleted_records += 1

        next_offset = len(pdbfile_in)
        for e in record_entries[i+1:]:
            if (e.attributes & dmRecAttrDelete == 0):
                next_offset = e.localChunkID
                break

        pack_into(">I", pdbfile_out, entry.offset, next_offset)

    print(f"fixed up {deleted_records} deleted records")

    try:
        with open(options.destination, 'bw') as writer:
            writer.write(pdbfile_out)

    except Exception as ex:
        print(f"could not write {options.destination}: {ex}")
        return

    print(f"wrote fixed database to {options.destination}")


parser = argparse.ArgumentParser(description="Tools for handling prc files")
subparsers = parser.add_subparsers(help="subcommand", dest="subcommand")

parserSplit = subparsers.add_parser(
    "split-resources", help="split into individual resources", description="Split the .prc into its individual resources")
parserSplit.add_argument("target", metavar="<target>",
                         type=str, help="target file")
parserSplit.add_argument("prefix", metavar="<prefix>",
                         type=str, help="target file")

parserFixPdb = subparsers.add_parser("fix-pdb", help="fix .pdb files",
                                     description="Fix record databases that were exported by CloudpilotEmu < 1.8.9 with PalmOS < 3 and that contain bad deleted records")
parserFixPdb.add_argument("source", metavar="<source>",
                          type=str, help="source file")
parserFixPdb.add_argument("destination", metavar="<destination>",
                          type=str, help="destination file")

options = parser.parse_args()

if options.subcommand == "split-resources":
    splitResources(options)

elif options.subcommand == "fix-pdb":
    fix_pdb(options)

else:
    parser.print_help()
