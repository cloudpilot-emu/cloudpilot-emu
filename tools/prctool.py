#!/usr/bin/env python3

import argparse
import datetime
import traceback
from struct import calcsize, pack, pack_into, unpack_from
from typing import Any, Callable, Self, cast

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

attributeValues = {
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


def decodeZeroTerminatedString(str: bytes) -> str:
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
    attributes: list[str] = []

    for name, value in attributeValues.items():
        if attr & value != 0:
            attributes.append(name)

    return ",".join(attributes)


def formatDate(d: int) -> str:
    date = datetime.datetime(year=1904, month=1, day=1) + \
        datetime.timedelta(seconds=d)

    return date.strftime("%m/%d/%Y, %H:%M:%S")


class UnknownSizeException(Exception):
    pass


class Struct:
    _format: str = ""

    def size(self) -> int:
        return calcsize(self._format)

    @classmethod
    def staticSize(self) -> int:
        return calcsize(self._format)

    def serialize(self) -> bytes:
        return b''


class Header(Struct):
    name: str
    attributes: int
    version: int
    creationDate: int
    modificationDate: int
    lastBackupDate: int
    modificationNumber: int
    appInfoId: int
    sortInfoId: int
    type: bytes
    creator: bytes
    uniqueIdSeed: int
    offset: int

    _format: str = ">32s2H6I4s4sI"

    def __init__(
        self,
        name: str,
        attributes: int,
        creationDate: int,
        modificationDate: int,
        lastBackupDate: int,
        type: bytes,
        creator: bytes,
        appInfoId: int = 0,
        sortInfoId: int = 0,
        uniqueIdSeed: int = 0,
        modificationNumber: int = 0,
        version: int = 0,
        offset: int = 0
    ):
        self.name = name
        self.attributes = attributes
        self.version = version
        self.creationDate = creationDate
        self.modificationDate = modificationDate
        self.lastBackupDate = lastBackupDate
        self.modificationNumber = modificationNumber
        self.appInfoId = appInfoId
        self.sortInfoId = sortInfoId
        self.type = type
        self.creator = creator
        self.uniqueIdSeed = uniqueIdSeed
        self.offset = offset

    @classmethod
    def parse(cls, buffer: bytes, offset: int) -> 'Header':
        (encodedName,
         attributes,
         version,
         creationDate,
         modificationDate,
         lastBackupDate,
         modificationNumber,
         appInfoId,
         sortInfoId,
         type,
         creator,
         uniqueIdSeed) = unpack_from(cls._format, buffer, offset)

        return cls(
            name=decodeZeroTerminatedString(encodedName),
            attributes=attributes,
            version=version,
            creationDate=creationDate,
            modificationDate=modificationDate,
            lastBackupDate=lastBackupDate,
            modificationNumber=modificationNumber,
            appInfoId=appInfoId,
            sortInfoId=sortInfoId,
            type=type,
            creator=creator,
            uniqueIdSeed=uniqueIdSeed,
            offset=offset
        )

    def serialize(self) -> bytes:
        return pack(self._format,
                    self.name[:31].encode('ascii') + b'\0',
                    self.attributes,
                    self.version,
                    self.creationDate,
                    self.modificationDate,
                    self.lastBackupDate,
                    self.modificationNumber,
                    self.appInfoId,
                    self.sortInfoId,
                    self.type,
                    self.creator,
                    self.uniqueIdSeed,
                    )

    def isResourceDb(self) -> bool:
        return self.attributes & dmHdrAttrResDB != 0

    def isRecordDb(self) -> bool:
        return self.attributes & dmHdrAttrResDB == 0


class RecordList(Struct):
    nextRecordListId: int
    numRecords: int
    offset: int

    _format: str = ">IH"

    def __init__(self, nextRecordListId: int, numRecords: int, offset: int = 0):
        self.nextRecordListId = nextRecordListId
        self.numRecords = numRecords
        self.offset = offset

    @classmethod
    def parse(cls, buffer: bytes, offset: int) -> 'RecordList':
        (nextRecordListId, numRecords) = unpack_from(
            cls._format, buffer, offset)

        return RecordList(nextRecordListId, numRecords, offset)

    def serialize(self) -> bytes:
        return pack(self._format, self.nextRecordListId, self.numRecords)


class Entry(Struct):
    localChunkId: int
    id: int


class ResourceEntry(Entry):
    type: bytes
    offset: int

    _format: str = ">4sHI"

    def __init__(self, type: bytes, id: int, localChunkId: int, offset: int = 0):
        self.type = type
        self.id = id
        self.localChunkId = localChunkId
        self.offset = offset

    @classmethod
    def parse(cls, buffer: bytes, offset: int) -> 'ResourceEntry':
        (type, id, localChunkId) = unpack_from(
            cls._format, buffer, offset)

        return ResourceEntry(type, id, localChunkId, offset)

    def serialize(self):
        return pack(self._format, self.type, self.id, self.localChunkId)


class RecordEntry(Entry):
    attributes: int
    offset: int

    _format: str = ">IB3s"

    def __init__(self, attributes: int, id: int, localChunkId: int, offset: int = 0):
        self.attributes = attributes
        self.id = id
        self.localChunkId = localChunkId
        self.offset = offset

    @classmethod
    def parse(cls, buffer: bytes, offset: int):
        (localChunkId, attributes, encodedId) = unpack_from(
            cls._format, buffer, offset)

        id = encodedId[2] | (
            encodedId[1] << 8) | (encodedId[0] << 16)

        return RecordEntry(attributes, id, localChunkId, offset)

    def serialize(self):
        encodedId = bytes([self.id & 0xff,
                           (self.id >> 8) & 0xff, (self.id >> 16) & 0xff])

        return pack(self._format, self.localChunkId, self.attributes, encodedId)


class Item[T: Entry]:
    id: int
    data: bytes

    def entry(self, localChunkId: int) -> T:
        raise NotImplementedError()


class Resource(Item[ResourceEntry]):
    type: bytes

    def __init__(self, type: bytes, id: int, data: bytes):
        self.type = type
        self.id = id
        self.data = data

    def entry(self, localChunkId: int) -> ResourceEntry:
        return ResourceEntry(self.type, self.id, localChunkId)


class Record(Item[RecordEntry]):
    attributes: int

    def __init__(self, attributes: int, uniqueId: int, data: bytes):
        self.attributes = attributes
        self.id = uniqueId
        self.data = data

    def entry(self, localChunkId) -> RecordEntry:
        return RecordEntry(self.attributes, self.id, localChunkId)


class Database[T: Item[Entry]]:
    name: str
    attributes: int
    version: int
    creationDate: int
    modificationDate: int
    lastBackupDate: int
    modificationNumber: int
    type: bytes
    creator: bytes
    uniqueIdSeed: int

    gapSize: int

    appInfo: bytes | None
    sortInfo: bytes | None

    items: list[T]

    def __init__(self,
                 name: str,
                 attributes: int,
                 creationDate: int,
                 modificationDate: int,
                 lastBackupDate: int,
                 type: bytes,
                 creator: bytes,
                 version: int = 0,
                 modificationNumber: int = 0,
                 uniqueIdSeed: int = 0,
                 gapSize: int = 2,
                 appInfo: bytes | None = None,
                 sortInfo: bytes | None = None,
                 ):
        self.name = name
        self.attributes = attributes
        self.creationDate = creationDate
        self.modificationDate = modificationDate
        self.lastBackupDate = lastBackupDate
        self.type = type
        self.creator = creator
        self.version = version
        self.modificationNumber = modificationNumber
        self.uniqueIdSeed = uniqueIdSeed
        self.gapSize = gapSize
        self.appInfo = appInfo
        self.sortInfo = sortInfo

        self.items = []

    @classmethod
    def fromHeader(cls, header: Header) -> 'Database[T]':
        return Database(
            name=header.name,
            attributes=header.attributes,
            version=header.version,
            creationDate=header.creationDate,
            lastBackupDate=header.lastBackupDate,
            modificationDate=header.modificationDate,
            type=header.type,
            creator=header.creator,
            uniqueIdSeed=header.uniqueIdSeed,
            modificationNumber=header.modificationNumber,
        )

    def add(self, item: T):
        self.items.append(item)

    def serialize(self) -> bytes:
        offset = Header.staticSize() + RecordList.staticSize() + self.gapSize

        appInfoId = 0
        if self.appInfo is not None:
            appInfoId = offset
            offset += len(self.appInfo)

        sortInfoId = 0
        if self.sortInfo is not None:
            sortInfoId = offset
            offset += len(self.sortInfo)

        header = Header(
            name=self.name[:31],
            attributes=self.attributes,
            version=self.version,
            creationDate=self.creationDate,
            modificationDate=self.modificationDate,
            lastBackupDate=self.lastBackupDate,
            modificationNumber=self.modificationNumber,
            appInfoId=appInfoId,
            sortInfoId=sortInfoId,
            type=options.type,
            creator=options.creator,
            uniqueIdSeed=self.uniqueIdSeed
        )

        recordList = RecordList(0, len(self.items))
        gap = bytes([0 for _ in range(0, self.gapSize)])

        serialized = header.serialize() + recordList.serialize() + gap

        if self.appInfo is not None:
            serialized += self.appInfo

        if self.sortInfo is not None:
            serialized += self.sortInfo

        for item in self.items:
            entry = item.entry(offset)

            offset += entry.size()
            serialized += entry.serialize()

        return serialized

    def isResourceDb(self) -> bool:
        return self.attributes & dmHdrAttrResDB != 0

    def isRecordDb(self) -> bool:
        return self.attributes & dmHdrAttrResDB == 0


class BadDatabaseException(Exception):
    reason: str

    def __init__(self, reason: str):
        self.reason = reason

    def __str__(self) -> str:
        return self.reason


def parseDb(data: bytes) -> Database:
    def appendRecords[T: Item, U: Entry](db: Database[T], recordList: RecordList, offset: int, watermark: int,
                                         parse: Callable[[bytes, int], U], create: Callable[[U, bytes], T]) -> int:
        firstBlockStart = len(data)

        for i in range(0, recordList.numRecords):
            entry = parse(data, offset)
            offset += entry.size()

            if i == 0:
                firstBlockStart = entry.localChunkId

            recordEnd = len(data) if (
                i == recordList.numRecords - 1) else parse(data, offset).localChunkId

            if recordEnd > len(data) or entry.localChunkId < watermark:
                raise BadDatabaseException(f'bad entry {i}')

            watermark = recordEnd

            db.add(create(entry, data[entry.localChunkId:recordEnd]))

        return firstBlockStart

    offset = 0

    header = Header.parse(data, offset)
    offset += header.size()

    recordList = RecordList.parse(data, offset)
    offset += recordList.size()

    if recordList.nextRecordListId != 0:
        raise RuntimeError("unable to parse multi-segmented DB")

    firstBlockStart = len(data)

    watermark = offset + recordList.numRecords * \
        (ResourceEntry.staticSize() if header.isResourceDb()
         else RecordEntry.staticSize())

    if header.appInfoId > 0:
        if header.appInfoId < watermark:
            raise BadDatabaseException("bad appinfo block")

        watermark = header.appInfoId

    if header.sortInfoId > 0:
        if header.sortInfoId < watermark:
            raise BadDatabaseException("bad sortinfo block")

        watermark = header.sortInfoId

    db: Database[Item]

    if header.isResourceDb():
        resourceDb = Database[Resource].fromHeader(header)

        appendRecords(resourceDb, recordList, offset, watermark,
                      parse=lambda data, offset: ResourceEntry.parse(
                          data, offset),
                      create=lambda entry, data: Resource(entry.type, entry.id, data))

        db = cast(Database[Item], resourceDb)

    else:
        recordDb = Database[Record].fromHeader(header)

        appendRecords(recordDb, recordList, offset, watermark,
                      parse=lambda data, offset: RecordEntry.parse(
                          data, offset),
                      create=lambda entry, data: Record(entry.attributes, entry.id, data))

        db = cast(Database[Item], recordDb)

    if header.sortInfoId > 0:
        db.sortInfo = data[header.sortInfoId:firstBlockStart]
        firstBlockStart = header.sortInfoId

    if header.appInfoId > 0:
        db.appInfo = data[header.appInfoId:firstBlockStart]
        firstBlockStart = header.appInfoId

    db.gapSize = firstBlockStart - offset

    return db


def splitResources(options: Any):
    try:
        db = parseDb(options.database)
    except Exception as ex:
        print(f'failed to parse {options.target}: {ex}')
        return

    if not db.isResourceDb():
        print('not a resource database!')
        return

    resources = cast(Database[Resource], db).items

    print(f'name: {db.name}')
    print(f'type: {formatTag(db.type)}')
    print(f'creator: {formatTag(db.creator)}')
    print(f'writing {len(resources)} resource entries')

    for resource in resources:
        filename = f'{options.prefix}{resource.type.decode('ascii')}_{
            resource.id}'
        print(f'writing {filename}')

        with open(filename, 'bw') as writer:
            writer.write(resource.data)


def fixPdb(options: Any):
    pdbfile_in: bytes = options.source

    pdbfile_out = bytearray(len(pdbfile_in))
    pdbfile_out[:] = pdbfile_in

    try:
        header = Header.parse(pdbfile_in, 0)
        offset = header.size()

        if not header.isRecordDb() != 0:
            print("not a record DB")
            return

        recordList = RecordList.parse(pdbfile_in, offset)
        offset += recordList.size()

    except Exception as ex:
        print(f"faile to parse {options.source}: {ex}")

    print(f"DB name: {header.name}")
    print(f"found {recordList.numRecords} records")

    record_entries: list[RecordEntry] = []

    for i in range(0, recordList.numRecords):
        entry = RecordEntry.parse(pdbfile_in, offset)
        offset += entry.size()

        record_entries.append(entry)

    deleted_records = 0

    for i, entry in enumerate(record_entries):
        if (entry.localChunkId != 0):
            continue

        deleted_records += 1

        next_offset = len(pdbfile_in)
        for e in record_entries[i+1:]:
            if (e.localChunkId != 0):
                next_offset = e.localChunkId
                break

        pack_into(">I", pdbfile_out, entry.offset, next_offset)

    print(f"fixed up {deleted_records} deleted records")

    try:
        with open(options.destination, 'bw') as writer:
            writer.write(pdbfile_out)

    except Exception as ex:
        print(f'could not write "{options.destination}"')
        return

    print(f'wrote fixed database to "{options.destination}"')


def dbInfo(options: Any):
    try:
        db = parseDb(options.source)

    except Exception as ex:
        print(f'failed to parse database: {ex}')
        return

    print(f"name: {db.name}")
    print(
        f"kind: {'resource' if db.isResourceDb() else 'record'}")
    print(f'type: {formatTag(db.type)}')
    print(f'creator: {formatTag(db.creator)}')
    print(f'created: {formatDate(db.creationDate)}')
    print(f'modified: {formatDate(db.modificationDate)}')
    print(f'last backup: {formatDate(db.lastBackupDate)}')
    print(f'attributes: {formatDbAttribtes(db.attributes)}')
    print(f'modification number: {db.modificationNumber}')
    print(f'seed: {db.uniqueIdSeed}')
    print("no appinfo" if db.appInfo is None else f'appinfo: {
        len(db.appInfo)} bytes')
    print("no sortinfo" if db.sortInfo is None else f'sortinfo: {
        len(db.sortInfo)} bytes')
    print(f'gap: {db.gapSize} bytes')
    print()

    if db.isResourceDb():
        print("no resources" if len(db.items) ==
              0 else f'{len(db.items)} resources')
    else:
        print("no records" if len(db.items) ==
              0 else f'{len(db.items)} records')

    if options.appinfo is not None and db.appInfo is not None:
        try:
            with open(options.appinfo, 'bw') as writer:
                writer.write(db.appInfo)

            print(f'wrote appinfo block to "{options.appinfo}"')

        except Exception as ex:
            print(f'could not write "{options.appinfo}"')
            return

    if options.sortinfo is not None and db.sortInfo is not None:
        try:
            with open(options.sortinfo, 'bw') as writer:
                writer.write(db.sortInfo)

            print(f'wrote sortinfo block to "{options.sortinfo}"')

        except Exception as ex:
            print(f'could not write "{options.sortinfo}"')
            return


def createDb(options: Any):
    attributes = 0 if options.attributes is None else options.attributes
    if options.resource:
        attributes |= dmHdrAttrResDB

    now = int((datetime.datetime.now() -
               datetime.datetime(year=1904, month=1, day=1)).total_seconds())

    db = Database[Item[Entry]](
        name=options.name,
        attributes=attributes,
        creationDate=now,
        modificationDate=now,
        lastBackupDate=now,
        type=options.type,
        creator=options.creator,
        gapSize=options.gap if options.gap > 0 else 0,
        appInfo=options.appinfo,
        sortInfo=options.sortinfo
    )

    try:
        with open(options.destination, 'bw') as writer:
            writer.write(db.serialize())

    except Exception as ex:
        print(f"could not write {options.destination}: {ex}")
        return

    print(f"wrote database to {options.destination}")


def typeTag(x: str):
    if len(x) != 4:
        raise argparse.ArgumentTypeError(
            "value must be exactly 4 ASCII characters")

    try:
        return x.encode('ascii')
    except UnicodeEncodeError:
        raise argparse.ArgumentTypeError("value must be ASCII")


def typeAttributes(list: str):
    validNames = ",".join(
        [name for name in attributeValues.keys() if name != "res"])

    msg = f'valid attributes are {validNames}'

    def convert(name: str) -> int:
        value = attributeValues.get(name)

        if value is None or name == "res":
            raise argparse.ArgumentTypeError(msg)

        return cast(int, value)

    value = 0
    for x in [convert(name) for name in list.split(",")]:
        value |= x

    return value


def typeFile(name: str):
    try:
        with open(name, "rb") as reader:
            return reader.read()

    except Exception:
        raise argparse.ArgumentTypeError(f'unable to read file "{name}"')


parser = argparse.ArgumentParser(description="Tools for handling prc files")
subparsers = parser.add_subparsers(help="subcommand", dest="subcommand")

parserSplit = subparsers.add_parser(
    "split-resources", help="split into individual resources", description="Split the .prc into its individual resources")
parserSplit.add_argument("database", metavar="<database file>",
                         type=typeFile, help="source database")
parserSplit.add_argument("prefix", metavar="<prefix>",
                         type=str, help="prefix for created resource files")

parserFixPdb = subparsers.add_parser("fix-pdb", help="fix .pdb files",
                                     description="Fix record databases that were exported by CloudpilotEmu < 1.8.9 with PalmOS < 3 and that contain bad deleted records")
parserFixPdb.add_argument("source", metavar="<source>",
                          type=typeFile, help="source file")
parserFixPdb.add_argument("destination", metavar="<destination>",
                          type=str, help="destination file")

parserDbInfo = subparsers.add_parser(
    'info', help="show DB info", description="show info and statistics on database")
parserDbInfo.add_argument("source", metavar="<source>",
                          type=typeFile, help="source file")
parserDbInfo.add_argument("--appinfo", metavar="<file>",
                          type=str, help="write appinfo block to <file>")
parserDbInfo.add_argument("--sortinfo", metavar="<file>",
                          type=str, help="write sortinfo block to <file>")

parserCreateDb = subparsers.add_parser(
    "create", help="create new empty DB", description="create a new empty database")


parserCreateDb.add_argument(
    "destination", metavar="<destination>", help="destination file")
parserCreateDb.add_argument("--resource",
                            default=False, action="store_true", help="create resource database")
parserCreateDb.add_argument(
    "--name", metavar="name", type=str, required=True, help="DB name (max 31 chars)")
parserCreateDb.add_argument(
    "--type", metavar="type", type=typeTag, required=True, help="DB type (exactly 4 ASCII chars)")
parserCreateDb.add_argument("--creator", type=typeTag, metavar="creator",
                            required=True, help="DB creator (exactly 4 ASCII chars)")
parserCreateDb.add_argument(
    "--attributes", type=typeAttributes, help="DB attributes")
parserCreateDb.add_argument("--gap", type=int, default=2, help="gap size")
parserCreateDb.add_argument(
    "--appinfo", metavar="file", type=typeFile, help="appinfo block")
parserCreateDb.add_argument(
    "--sortinfo", metavar="file", type=typeFile, help="sortinfo block")

options = parser.parse_args()

if options.subcommand == "split-resources":
    splitResources(options)

elif options.subcommand == "fix-pdb":
    fixPdb(options)

elif options.subcommand == "info":
    dbInfo(options)

elif options.subcommand == "create":
    createDb(options)

else:
    parser.print_help()
