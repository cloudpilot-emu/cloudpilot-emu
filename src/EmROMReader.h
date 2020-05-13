/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmROMReader_h
#define EmROMReader_h

/*
        This file defines EmROMReader, a class used to parse up raw ROM
        images and return information about them, such as the version number
        in the version resource, card header information, and whether or not
        the splash screen is in color.  This information is used when trying
        to determine on what device or devices the ROM can run.

        NOTE: this class used to be a template class, parameterized on LAS
        and PAS in EmPalmStructs.h.  The intent was that it could be used to
        access a raw ROM image from disk (using LAS) or the ROM image as
        utilized in the emulated address space (using PAS).  However, VC++
        had problems trying to compile the nested classes in the EmROMReader
        class template, so that feature was sacrificed.
*/

#include "EmPalmStructs.h"

// Define this to 1 if you want to dump a list of databases
// to the log file as the ROM is being enumerated.  This is
// handy when trying to find something in the ROM that differentiates
// it from other ROMs.

#define LOG_DATABASES_HACK 0

#if LOG_DATABASES_HACK
    #include "Logging.h"  // LogAppendMsg
#endif

#include <map>
#include <string>
#include <vector>

typedef LAS A;

class EmROMReader {
   public:
    typedef A::ptr_type ptr_type;

    class ChunkType {
       public:
        ChunkType(void);
        ChunkType(const EmAliasROMHeapChunkHdr2Type<A>& chunk, UInt32 addr);
        ChunkType(const EmAliasROMHeapChunkHdr1Type<A>& chunk, UInt32 addr);

       public:
        bool Free(void) { return fFree; }
        int Owner(void) { return fOwner; }
        unsigned long TotalSize(void) { return fSize; }
        size_t Size(void) { return fSize - fHdrSize - fSizeAdj; }
        ptr_type GetHdrPtr(void) { return fPtr; }
        ptr_type GetPtr(void) { return A::add(fPtr, (int)(fHdrSize)); }
        UInt32 LocalID(void) { return fAddress; }

        bool Contains(ptr_type pos, size_t len = 1) {
            return (A::diff(pos, fPtr) >= 0) && ((A::diff(pos, fPtr) + len) <= fSize);
        }

       private:
        friend class EmROMReader;

        unsigned long fSize;
        bool fFree;
        unsigned long fSizeAdj;
        int fLockCount;
        int fHOffset;
        int fOwner;
        ptr_type fPtr;
        unsigned long fAddress;
        int fHdrSize;

        unsigned long recordNumber;
        unsigned long dbNumber;

        enum { typeOther, typeRecord, typeDBHeader, typeDBTable, typeFree } type;
    };

    typedef map<unsigned long, ChunkType> ChunkList;

    class DatabaseItemType {
       public:
        DatabaseItemType(void);
        DatabaseItemType(const EmAliasROMDBRecordHeader1Type<A> record, EmROMReader* rom);
        DatabaseItemType(const EmAliasROMDBResourceHeader1Type<A> resource, EmROMReader* rom);

       public:
        bool Deleted(void) { return fDeleted; }
        bool Dirty(void) { return fDirty; }
        bool Busy(void) { return fBusy; }
        bool Secret(void) { return fSecret; }
        int Category(void) { return fCategory; }
        unsigned long ID(void) { return fID; }
        unsigned long Type(void) { return fType; }
        bool Record(void) { return fRecord; }
        ChunkType* Chunk(void) { return fChunk; }

       private:
        friend class EmROMReader;

        bool fRecord;
        unsigned long fID;
        unsigned long fType;
        int fCategory;
        bool fDeleted;
        bool fDirty;
        bool fBusy;
        bool fSecret;
        UInt32 fChunkLID;

        ChunkType* fChunk;
    };

    typedef vector<DatabaseItemType> DatabaseItemList;

    class DatabaseType {
       public:
        DatabaseType(void);
        DatabaseType(ChunkType* hdr, EmROMReader* rom);

       public:
        string& Name(void) { return fName; }
        const string& Name(void) const { return fName; }
        DatabaseItemList& Items(void) { return fItems; }

        unsigned long CreationTime(void) { return fCreationTime; }
        unsigned long ModificationTime(void) { return fModificationTime; }
        unsigned long BackupTime(void) { return fBackupTime; }
        unsigned int Flags(void) { return fFlags; }
        unsigned int Version(void) { return fVersion; }
        unsigned long UniqueIDSeed(void) { return fUniqueIDSeed; }

        unsigned long Creator(void) { return fCreator; }
        unsigned long Type(void) { return fType; }

       private:
        friend class EmROMReader;

        DatabaseItemList fItems;
        string fName;
        unsigned long fCreator, fType;
        unsigned long fCreationTime, fModificationTime, fBackupTime;
        unsigned int fFlags, fVersion;
        unsigned long fUniqueIDSeed;
        ChunkType* fAppInfoChunk;
        ChunkType* fSortInfoChunk;
    };

    typedef vector<DatabaseType> DatabaseList;
    typedef map<unsigned long, map<int, int> > FeatureList;

   public:
    EmROMReader(ptr_type arenaPtr, size_t arenaSize);
    ~EmROMReader(void);

    bool AcquireCardHeader(void);
    UInt32 GuessCardAddress(void);
    void SetCardAddress(UInt32 address);
    bool AcquireROMHeap(void);
    bool AcquireDatabases(void);
    bool AcquireFeatures(void);
    bool AcquireSplashDB(void);

    bool ContainsDB(string dbName) const;

    void Version(int& major, int& minor, int& fix, int& stage, int& build) const;
    unsigned long Version(void) const;

    static int IsBitmapColor(ChunkType& chunk, bool bootScreen);

   public:
    ChunkList& Chunks(void) { return fChunks; }
    DatabaseList& Databases(void) { return fDatabases; }
    FeatureList& Features(void) { return fFeatures; }
    ChunkType* GetSplashChunk(void) const { return fSplashChunk; }
    ptr_type GetCardHeaderPtr(void) const { return fPtr; }
    ptr_type GetCardStorePtr(void) const {
        return A::add(fPtr, EmAliasROMCardHeader5Type<A>::GetSize());
    }
    bool GetFlag328(void) const { return fFlag328; }
    bool GetFlagEZ(void) const { return fFlagEZ; }
    bool GetFlagVZ(void) const { return fFlagVZ; }
    bool GetFlagSZ(void) const { return fFlagSZ; }
    UInt16 GetCardVersion(void) const { return fCardVersion; }
    string GetCardName(void) const { return fCardName; }
    string GetCardManufacturer(void) const { return fCardManuf; }
    UInt16 GetStoreVersion(void) const { return fStoreVersion; }
    UInt32 GetCompanyID(void) const { return fCompanyID; }
    UInt32 GetHalID(void) const { return fHalID; }
    UInt32 GetRomVersion(void) const { return fRomVersion; }
    string GetRomVersionString(void) const { return fRomVersionString; }

   private:
    ptr_type fPtr;
    size_t fSize;
    ptr_type fEndPtr;

    UInt32 fCardAddress;

    ChunkList fChunks;
    DatabaseList fDatabases;
    FeatureList fFeatures;

    bool fFlag328;
    bool fFlagEZ;
    bool fFlagVZ;
    bool fFlagSZ;

    UInt16 fCardVersion;
    string fCardName, fCardManuf;

    UInt16 fStoreVersion;

    UInt32 fCompanyID, fHalID;
    UInt32 fRomVersion;
    string fRomVersionString;

    ChunkType* fSplashChunk;
};

#endif  // EmROMReader_h
