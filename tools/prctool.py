#!/usr/bin/env python3

import argparse
import datetime
from struct import calcsize, pack_into, unpack_from

dmHdrAttrResDB = 0x0001
dmHdrAttrReadOnly = 0x0002
dmHdrAttrAppInfoDirty = 0x0004
dmHdrAttrBackup = 0x0008
dmHdrAttrOKToInstallNewer = 0x0010
dmHdrAttrResetAfterInstall = 0x0020
dmHdrAttrCopyPrevention = 0x0040
dmHdrAttrStream = 0x0080
dmHdrAttrHidden = 0x0100
dmHdrAttrLaunchableData = 0x0200
dmHdrAttrOpen = 0x8000


def transformZeroTerminatedString(str: bytes) -> str:
    terminator = str.find(0)

    return str[:terminator if terminator >= 0 else len(str)].decode('ascii')


def formatTag(tag: bytes) -> str:
    formatted = ''

    for i in tag:
        if i >= 0x21 and i <= 0x7e:
            formatted += chr(i)

        else:
            formatted += f'[i]'

    return formatted


def formatDbAttribtes(attr: int) -> str:
    mappings = {
        "res": dmHdrAttrResDB,
        "readonly": dmHdrAttrReadOnly,
        "appinfoDirty": dmHdrAttrAppInfoDirty,
        "backup": dmHdrAttrBackup,
        "overwriteNewer": dmHdrAttrOKToInstallNewer,
        "reset": dmHdrAttrResetAfterInstall,
        "protect": dmHdrAttrCopyPrevention,
        "stream": dmHdrAttrStream,
        "hidden": dmHdrAttrHidden,
        "launchable": dmHdrAttrLaunchableData,
        "open": dmHdrAttrOpen
    }

    attributes: list[str] = []

    for name, value in mappings.items():
        if attr & value != 0:
            attributes.append(name)

    return ",".join(attributes)


def formatDate(d: int) -> str:
    date = datetime.datetime(year=1904, month=1, day=1) + \
        datetime.timedelta(seconds=d)

    return date.strftime("%m/%d/%Y, %H:%M:%S")


class Struct:
    _format: str = ""

    def size(self) -> int:
        return calcsize(self._format)


class Header(Struct):
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
         self.modificationNumber,
         self.appInfoID,
         self.sortInfoID,
         self.type,
         self.creator,
         self.uniqueIDSeed) = unpack_from(self._format, buffer, offset)

        self.offset = offset

        self.name = transformZeroTerminatedString(encodedName)


class RecordList(Struct):
    nextRecordListID: int
    numRecords: int
    offset: int

    _format: str = ">IH"

    def __init__(self, buffer: bytes, offset: int):
        (self.nextRecordListID, self.numRecords) = unpack_from(
            self._format, buffer, offset)

        self.offset = offset

    def size(self) -> int:
        return 6 if self.numRecords > 0 else 8


class ResourceEntry(Struct):
    type: bytes
    id: int
    localChunkID: int
    offset: int

    _format: str = ">4sHI"

    def __init__(self, buffer: bytes, offset: int):
        (self.type, self.id, self.localChunkID) = unpack_from(
            self._format, buffer, offset)

        self.offset = offset


class RecordEntry(Struct):
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


class Database:
    name: str
    attributes: int
    version: int
    creationDate: int
    modificationDate: int
    lastBackupDate: int
    modificationNumber: int
    type: bytes
    creator: bytes
    uniqueIDSeed: int

    gapSize: int

    appInfo: bytes | None
    sortInfo: bytes | None

    def __init__(self, header: Header):
        self.name = header.name
        self.attributes = header.attributes
        self.version = header.version
        self.creationDate = header.creationDate
        self.lastBackupDate = header.lastBackupDate
        self.modificationDate = header.modificationDate
        self.type = header.type
        self.creator = header.creator
        self.uniqueIDSeed = header.uniqueIDSeed
        self.modificationNumber = header.modificationNumber

        self.gapSize = 2
        self.appInfo = None
        self.sortInfo = None


class Resource:
    type: bytes
    id: int
    data: bytes

    def __init__(self, type: bytes, id: int, data: bytes):
        self.type = type
        self.id = id
        self.data = data


class Record:
    attributes: int
    unique_id: int
    data: bytes

    def __init__(self, attributes: int, unique_id: int, data: bytes):
        self.attributes = attributes
        self.unique_id = unique_id
        self.data = data


class ResourceDb(Database):
    resources: list[Resource]

    def __init__(self, header: Header):
        super().__init__(header)

        self.resources = []

    def addResource(self, resource: Resource):
        self.resources.append(resource)


class RecordDb(Database):
    records: list[Record]

    def __init__(self, header: Header):
        super().__init__(header)

        self.records = []

    def addRecord(self, record: Record):
        self.records.append(record)


class BadDatabaseException(Exception):
    reason: str

    def __init__(self, reason: str):
        self.reason = reason

    def __str__(self) -> str:
        return self.reason


def parseDb(data: bytes) -> Database:
    offset = 0

    header = Header(data, offset)
    offset += header.size()

    recordList = RecordList(data, offset)
    offset += recordList.size()

    if recordList.nextRecordListID != 0:
        raise RuntimeError("unable to parse multi-segmented DB")

    firstBlockStart = len(data)

    isResourceDb = header.attributes & dmHdrAttrResDB != 0
    watermark = offset + recordList.numRecords * (10 if isResourceDb else 8)

    if header.appInfoID > 0:
        if header.appInfoID < watermark:
            raise BadDatabaseException("bad appinfo block")

        watermark = header.appInfoID

    if header.sortInfoID > 0:
        if header.sortInfoID < watermark:
            raise BadDatabaseException("bad sortinfo block")

        watermark = header.sortInfoID

    db: Database

    if isResourceDb:
        resourceDb = ResourceDb(header)

        for i in range(0, recordList.numRecords):
            rsc = ResourceEntry(data, offset)
            offset += rsc.size()

            if i == 0:
                firstBlockStart = rsc.localChunkID

            recordEnd = len(data) if (
                i == recordList.numRecords - 1) else ResourceEntry(data, offset).localChunkID

            if recordEnd > len(data) or rsc.localChunkID < watermark:
                raise BadDatabaseException(f'bad record {i}')

            watermark = recordEnd

            resourceDb.addResource(Resource(rsc.type, rsc.id,
                                            data[rsc.localChunkID:recordEnd]))

        db = resourceDb

    else:
        recordDb = RecordDb(header)

        for i in range(0, recordList.numRecords):
            rec = RecordEntry(data, offset)
            offset += rec.size()

            if i == 0:
                firstBlockStart = rec.localChunkID

            recordEnd = len(data) if (
                i == recordList.numRecords - 1) else RecordEntry(data, offset).localChunkID

            if recordEnd > len(data) or rec.localChunkID < watermark:
                raise BadDatabaseException(f'bad record {i}')

            watermark = recordEnd

            recordDb.addRecord(
                Record(rec.attributes, rec.unique_id, data[rec.localChunkID:recordEnd]))

        db = recordDb

    if header.sortInfoID > 0:
        db.sortInfo = data[header.sortInfoID:firstBlockStart]
        firstBlockStart = header.sortInfoID

    if header.appInfoID > 0:
        db.appInfo = data[header.appInfoID:firstBlockStart]
        firstBlockStart = header.appInfoID

    db.gapSize = firstBlockStart - offset

    return db


def splitResources(options):
    try:
        with open(options.target, "rb") as reader:
            prcfile: bytes = reader.read()

    except Exception:
        print(f'unable to read {options.target}')
        return

    try:
        db = parseDb(prcfile)
    except Exception as ex:
        print(f'failed to parse {options.target}: {ex}')
        return

    if not isinstance(db, ResourceDb):
        print(f'{options.target} is not a resource database')
        return

    print(f'name: {db.name}')
    print(f'type: {formatTag(db.type)}')
    print(f'creator: {formatTag(db.creator)}')
    print(f'writing {len(db.resources)} resource entries')

    for resource in db.resources:
        filename = f'{options.prefix}{resource.type.decode('ascii')}_{resource.id}'
        print(f'writing {filename}')

        with open(filename, 'bw') as writer:
            writer.write(resource.data)


def fixPdb(options):
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
        if (entry.localChunkID != 0):
            continue

        deleted_records += 1

        next_offset = len(pdbfile_in)
        for e in record_entries[i+1:]:
            if (e.localChunkID != 0):
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


def dbInfo(options):
    try:
        with open(options.source, "rb") as reader:
            data = reader.read()

    except Exception:
        print(f'unable to read {options.source}')
        return

    try:
        db = parseDb(data)

        print(f"name: {db.name}")
        print(
            f"kind: {'resource' if isinstance(db, ResourceDb) else 'record'}")
        print(f'type: {formatTag(db.type)}')
        print(f'creator: {formatTag(db.creator)}')
        print(f'created: {formatDate(db.creationDate)}')
        print(f'modified: {formatDate(db.modificationDate)}')
        print(f'last backup: {formatDate(db.lastBackupDate)}')
        print(f'attributes: {formatDbAttribtes(db.attributes)}')
        print(f'modification number: {db.modificationNumber}')
        print(f'seed: {db.uniqueIDSeed}')
        print("no appinfo" if db.appInfo ==
              None else f'appinfo: {len(db.appInfo)} bytes')
        print("no sortinfo" if db.sortInfo ==
              None else f'sortinfo: {len(db.sortInfo)} bytes')
        print(f'gap: {db.gapSize} bytes')

    except Exception as ex:
        print(f'failed to parse database: {ex}')


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

parserDbInfo = subparsers.add_parser(
    'info', help="show DB info", description="show info and statistics on database")

parserDbInfo.add_argument("source", metavar="<source>",
                          type=str, help="source file")

options = parser.parse_args()

if options.subcommand == "split-resources":
    splitResources(options)

elif options.subcommand == "fix-pdb":
    fixPdb(options)

elif options.subcommand == "info":
    dbInfo(options)

else:
    parser.print_help()
