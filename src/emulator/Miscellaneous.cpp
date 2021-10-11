#include "Miscellaneous.h"

#include "Byteswapping.h"
#include "EmBankMapped.h"
#include "EmLowMem.h"
#include "EmMemory.h"
#include "EmPalmFunction.h"
#include "EmPalmStructs.h"
#include "EmPatchModuleHtal.h"
#include "EmSystemState.h"
#include "Feature.h"
#include "Logging.h"
#include "Platform.h"
#include "ROMStubs.h"
#include "UAE.h"

#define LOGGING 1
#ifdef LOGGING
    #define PRINTF logging::printf
#else
    #define PRINTF(...) ;
#endif

#define dataAppInfoVersionSize 16
#define dataAppInfoSignature 'lnch'
#define dataAppInfoVersion 3

namespace {
    bool AppCompareDataBaseNames(const DatabaseInfo& a, const DatabaseInfo& b) {
        return strcasecmp(a.dbName, b.dbName) < 0;
    }
    struct DataAppInfoType {
        UInt32 signature;    // must be 'lnch' (0x6C6E6338)
        UInt16 hdrVersion;   // version of this header - must be 3
        UInt16 encVersion;   // encoder version
        UInt16 verStrWords;  // length of version string array that
                             //	follows in 16-bit words.
        // UInt8			verStr[verStrWords];		// 0 terminated version
        // string with 	possible extra NULL byte at end for 	padding

        //--- The title is only present in version 2 or later
        UInt16 titleWords;  // length of title string array that follows
                            //	 in 16-bit words.
        // UInt8			title[titleWords];			// 0 terminated
        // title string with possible 	extra NULL at end for padding.

        UInt16 iconWords;  // length of icon data that follows in 16-bit
                           //	 words.
        // UInt8			icon[iconWords];			// icon in
        // "BitmapType" format with possible NULL 	byte at end for even UInt16 padding
        UInt16 smIconWords;  // length of small icon data that follows in
                             //	 16-bit words
                             // UInt8			smIcon[smIconWords];		// small
                             // icon in "BitmapType" format
        // with
        // possible NULL byte at end for even UInt16
        //	padding
        //--------- Version 2 Fields ------------
    };

    Bool IsVisible(UInt32 dbType, UInt32 dbCreator, UInt16 dbAttrs) {
        UNUSED_PARAM(dbCreator)

        // Don't show anything concerning the Launcher
        // (That comment and the following commented out code was from the
        // Launcher application. I've take it out so that we can run
        // Gremlins over the Launcher).
        //	if (dbCreator == sysFileCLauncher)
        //		return false;

        // The following test can come and go.	Currently, it's here
        // so that things like Clipper don't show up in the list (just
        // as it doesn't show up in the Launcher).	However, there may
        // be time when we want to show it.  An example would be in
        // an experiemental version of the New Gremlin dialog that
        // separated apps and documents.  Selecting an app in one list
        // would display a list of its documents in the other list.  In
        // that case, we'd need to show clipper in order to be able to
        // display its documents.

        // OK, the test is now gone.  From Scott Johnson:
        //
        //		The New Gremlin list doesn't show apps with the dmHdrAttrHidden attribute.
        //		This is a problem for mine, which is a sort of runtime forms engine.  The
        //		runtime is a hidden app and the user-built apps are visible.  The user app
        //		launches by just doing an app switch to the runtime.  (Sort of a launchable
        //		database concept for pre-3.2 systems.)  To Gremlin this, both apps need to
        //		be selected in the New Gremlin list.  But the hidden one isn't shown.  Oops.

        //	if (dbAttrs & dmHdrAttrHidden)
        //		return false;

        if (dbAttrs & dmHdrAttrLaunchableData) return true;

        if (dbType == sysFileTApplication) return true;

        if (dbType == sysFileTPanel) return true;

        return false;
    }

    Bool IsExecutable(UInt32 dbType, UInt32 dbCreator, UInt16 dbAttrs) {
        UNUSED_PARAM(dbCreator)
        UNUSED_PARAM(dbAttrs)

        if (dbType == sysFileTApplication) return true;

        if (dbType == sysFileTPanel) return true;

        return false;
    }

    static Err AppGetExtraInfo(DatabaseInfo* infoP) {
        Err err = errNone;

        infoP->name[0] = 0;

        //====================================================================
        // If it's a resource database, we must open it to get the appName
        //====================================================================
        if (infoP->dbAttrs & dmHdrAttrResDB) {
            emuptr appDB = 0;
            emuptr strH;

            // Open database
            appDB = DmOpenDatabase(infoP->cardNo, infoP->dbID, dmModeReadOnly);
            if (appDB == 0) {
                err = DmGetLastErr();
                goto Exit;
            }

            //...............................
            // Get app name if we don't already have it.
            //...............................
            strH = DmGet1Resource(ainRsc, ainID);

            // copy launcher name, if present
            if (strH != 0) {
                emuptr strP = (emuptr)MemHandleLock(strH);
                EmMem_strcpy(infoP->name, strP);
                MemHandleUnlock(strH);
                DmReleaseResource(strH);
            }

            ::DmCloseDatabase(appDB);
        }  // if resource database

        //====================================================================
        // If it's a record database, we look in the appInfo block.
        //====================================================================
        else {
            LocalID appInfoID;
            emuptr appInfoH = 0;
            emuptr appInfoP = 0;
            emuptr specialInfoP;
            emuptr bP;
            UInt16 verStrWords, titleWords;

            // Look for app info
            err = DmDatabaseInfo(infoP->cardNo, infoP->dbID, 0, 0, 0, 0, 0, 0, 0, &appInfoID, 0, 0,
                                 0);

            if (!err && appInfoID) {
                // Get handle (if RAM based) and ptr to app Info
                if (MemLocalIDKind(appInfoID) == memIDHandle) {
                    appInfoH = MemLocalIDToGlobal(appInfoID, infoP->cardNo);
                    appInfoP = MemHandleLock(appInfoH);
                } else {
                    appInfoP = MemLocalIDToGlobal(appInfoID, infoP->cardNo);
                }

                // See if this is the special launcher info and if so, get the icons
                //	out of that.
                specialInfoP = (emuptr)appInfoP;
                DataAppInfoType specialInfo;

                specialInfo.signature = EmMemGet32(specialInfoP);
                specialInfo.hdrVersion = EmMemGet16(specialInfoP + 4);
                specialInfo.encVersion = EmMemGet16(specialInfoP + 6);

                if (MemPtrSize(appInfoP) >= dataAppInfoVersionSize &&
                    specialInfo.signature == dataAppInfoSignature &&
                    specialInfo.hdrVersion >= dataAppInfoVersion) {
                    // Get ptr to version string
                    bP = specialInfoP + 8;
                    verStrWords = EmMemGet16(bP);
                    bP += 2;
                    bP += verStrWords * 2;

                    // Get ptr to name string
                    titleWords = EmMemGet16(bP);
                    bP += 2;
                    if (titleWords) {
                        EmMem_strcpy(infoP->name, bP);
                    }
                }  // If valid appInfo

                if (appInfoH) {
                    MemHandleUnlock(appInfoH);
                }
            }  // if (!err && appInfoID)
        }      // Record Database.

    Exit:

        // If no luck getting the visible name, put in default
        if (infoP->name[0] == 0) {
            // Get DB name
            strcpy(infoP->name, infoP->dbName);
        }

        return err;
    }

}  // namespace

uint32 NextPowerOf2(uint32 n) {
    // Smear down the upper 1 bit to all bits lower than it.

    uint32 n2 = n;

    n2 |= n2 >> 1;
    n2 |= n2 >> 2;
    n2 |= n2 >> 4;
    n2 |= n2 >> 8;
    n2 |= n2 >> 16;

    // Now use itself to clear all the lower bits.

    n2 &= ~(n2 >> 1);

    // If n2 ends up being the same as what we started with, keep it.
    // Otherwise, we need to bump it by a factor of two (round up).

    if (n2 != n) n2 <<= 1;

    return n2;
}

/***********************************************************************
 *
 * FUNCTION:	GetSystemCallContext
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool GetSystemCallContext(emuptr pc, SystemCallContext& context) {
    context.fPC = pc;

    // Determine how the system function is being called.  There are two ways:
    //
    //		* Via SYS_TRAP macro:
    //
    //			TRAP	$F
    //			DC.W	$Axxx
    //
    //		* Via SYS_TRAP_FAST macro:
    //
    //			MOVE.L	struct(LowMemType.fixed.globals.sysDispatchTableP), A1
    //			MOVE.L	((trapNum-sysTrapBase)*4)(A1), A1
    //			JSR 	(A1)	; opcode == 0x4e91
    //
    // The PC is current pointing to either the TRAP $F or the JSR (A1),
    // so we can look at the opcode to determine how we got here.

    uint16 opcode = EmMemGet16(pc);

    context.fViaTrap = opcode == (m68kTrapInstr + sysDispatchTrapNum);
    context.fViaJsrA1 = opcode == (0x4e91);

    if (context.fViaTrap) {
        // Not all development systems generate the correct dispatch
        // numbers; some leave off the preceding "A".  Make sure it's
        // set so that we can recognize it as a trap dispatch number.
        // (This code is here specifically so that the profiling routines
        //	will work, which check for trap numbers masquerading as function
        //	addresses by checking to see if they are in the sysTrapBase range.)

        context.fTrapWord = EmMemGet16(pc + 2) | sysTrapBase;
        context.fNextPC = pc + 4;
    } else if (context.fViaJsrA1) {
        context.fTrapWord = (EmMemGet16(pc - 2) / 4) | sysTrapBase;
        context.fNextPC = pc + 2;
    } else {
        EmAssert(false);
        return false;
    }

    if (::IsSystemTrap(context.fTrapWord)) {
        context.fTrapIndex = SysTrapIndex(context.fTrapWord);
        context.fExtra = m68k_dreg(regs, 2);
    } else {
        context.fTrapIndex = LibTrapIndex(context.fTrapWord);
        context.fExtra = EmMemGet16(m68k_areg(regs, 7));
    }

    EmAssert((context.fTrapWord >= sysTrapBase) && (context.fTrapWord < sysTrapBase + 0x1000));

    return true;
}

/***********************************************************************
 *
 * FUNCTION:    SeparateList
 *
 * DESCRIPTION: Break up a comma-delimited list of items, returning the
 *				pieces in a StringList.
 *
 * PARAMETERS:  stringList - the StringList to receive the broken-up
 *					pieces of the comma-delimited list.  This collection
 *					is *not* first cleared out, so it's possible to add
 *					to the collection with this function.
 *
 *				str - the string containing the comma-delimited items.
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void SeparateList(StringList& stringList, string str, char delimiter) {
    string::size_type offset;

    while ((offset = str.find(delimiter)) != string::npos) {
        string nextElement = str.substr(0, offset);
        str = str.substr(offset + 1);
        stringList.push_back(nextElement);
    }

    stringList.push_back(str);
}

/***********************************************************************
 *
 * FUNCTION:	EndsWith
 *
 * DESCRIPTION:	Determine if a string end with the given pattern.
 *
 * PARAMETERS:	s - string to test.
 *
 *				p - pattern to test with.
 *
 * RETURNED:	True if "s" ends with "p".
 *
 ***********************************************************************/

Bool EndsWith(const char* s, const char* p) {
    if (strlen(s) < strlen(p)) return false;

    const char* buffer = s + strlen(s) - strlen(p);
    return (strcasecmp(buffer, p) == 0);
}

StMemoryMapper::StMemoryMapper(const void* memory, long size) : fMemory(memory) {
    if (fMemory) Memory::MapPhysicalMemory(fMemory, size);
}

StMemoryMapper::~StMemoryMapper(void) {
    if (fMemory) Memory::UnmapPhysicalMemory(fMemory);
}

/***********************************************************************
 *
 * FUNCTION:	GetLibraryName
 *
 * DESCRIPTION:
 *
 * PARAMETERS:	none
 *
 * RETURNED:	The libraries name, or an empty string if the library
 *				could not be found.
 *
 ***********************************************************************/

string GetLibraryName(uint16 refNum) {
    if (refNum == sysInvalidRefNum) return string();

    CEnableFullAccess munge;  // Remove blocks on memory access.

    /*
            The System Library Table (sysLibTableP) is an array of
            sysLibTableEntries entries.  Each entry has the following
            format:

                    Ptr*		dispatchTblP;	// pointer to library dispatch table
                    void*		globalsP;		// Library globals
                    LocalID 	dbID;			// database id of the library
                    MemPtr	 	codeRscH;		// library code resource handle for
       RAM-based libraries

            The latter two fields are present only in Palm OS 2.0 and
            later.	So our first steps are to (a) get the pointer to
            the array, (b) make sure that the index into the array (the
            refNum passed as the first parameter to all library calls)
            is within range, (c) get a pointer to the right entry,
            taking into account the Palm OS version, and (d) getting the
            dispatchTblP field.

            The "library dispatch table" is an array of 16-bit offsets.  The
            values are all relative to the beginning of the table (dispatchTblP).
            The first entry in the array corresponds to the library name.  All
            subsequent entries are offsets to the various library functions,
            starting with the required four: sysLibTrapOpen, sysLibTrapClose,
            sysLibTrapSleep, and sysLibTrapWake.
    */

    emuptr sysLibTableP = EmLowMem_GetGlobal(sysLibTableP);
    UInt16 sysLibTableEntries = EmLowMem_GetGlobal(sysLibTableEntries);

    if (sysLibTableP == EmMemNULL) {
        // !!! No library table!
        EmAssert(false);
        return string();
    }

    if (refNum >= sysLibTableEntries) {
        if (refNum != 0x0666) {
            // !!! RefNum out of range!
            EmAssert(false);
        }

        return string();
    }

    // emuptr libEntry;
    emuptr dispatchTblP;

    if (gSystemState.OSMajorVersion() > 1) {
        EmAliasSysLibTblEntryType<PAS> libEntries(sysLibTableP);
        dispatchTblP = libEntries[refNum].dispatchTblP;
    } else {
        EmAliasSysLibTblEntryTypeV10<PAS> libEntries(sysLibTableP);
        dispatchTblP = libEntries[refNum].dispatchTblP;
    }

    // The first entry in the table is always the offset from the
    // start of the table to the library name.	Use this information
    // get the library name.

    int16 offset = EmMemGet16(dispatchTblP + LibTrapIndex(sysLibTrapName) * 2);
    emuptr libNameP = dispatchTblP + offset;

    char libName[256];
    EmMem_strcpy(libName, libNameP);

    PRINTF("library %u = %s", refNum, libName);

    return string(libName);
}

/***********************************************************************
 *
 * FUNCTION:	DateToDays
 *
 * DESCRIPTION: Convert a year, month, and day into the number of days
 *				since 1/1/1904.
 *
 *				Parameters are not checked for valid dates, so it's
 *				possible to feed in things like March 35, 1958.  This
 *				function also assumes that year is at least 1904, and
 *				will only work up until 2040 or so.
 *
 * PARAMETERS:	year - full year
 *
 *				month - 1..12
 *
 *				day - 1..31
 *
 * RETURNED:	Number of days since 1/1/1904.
 *
 ***********************************************************************/

uint32 DateToDays(uint32 year, uint32 month, uint32 day) {
    static const int month2days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    // Normalize the values.

    year -= 1904;
    month -= 1;
    day -= 1;

    // Not counting any possible leap-day in the current year, figure out
    // the number of days between now and 1/1/1904.

    const uint32 kNumDaysInLeapCycle = 4 * 365 + 1;

    uint32 days = day + month2days[month] + (year * kNumDaysInLeapCycle + 3) / 4;

    // Now add in this year's leap-day, if there is one.

    if ((month >= 2) && ((year & 3) == 0)) days++;

    return days;
}

void SetHotSyncUserName(const char* userNameP) {
    if (!Feature::GetHotsyncNameManagement()) return;

    if (EmLowMem::GetTrapAddress(sysTrapDlkDispatchRequest) == EmMemNULL) return;

    if (!userNameP) return;

    size_t userNameLen = strlen(userNameP) + 1;

    // If the name is too long, just return.  This should really only
    // happen if the user hand-edits the preference file to contain
    // a name that's too long.  The Preferences dialog box handler
    // checks as well, so the name shouldn't get too long from that path.

    if (userNameLen > dlkMaxUserNameLength + 1) return;

    // We need to prepare a command block for the DataLink Manager.
    // Define one large enough to hold all the data we'll pass in.
    //
    // The format of the data block is as follows:
    //
    //		[byte] DlpReqHeaderType.id			: Command request number (==
    // dlpWriteUserInfo) 		[byte] DlpReqHeaderType.argc		: # of arguments for
    // this command
    //(== 1)
    //
    //		[byte] DlpTinyArgWrapperType.bID	: ID of first argument (==
    // dlpWriteUserInfoReqArgID) 		[byte] DlpTinyArgWrapperType.bSize	: Size in
    // bytes of first argument (== whatever)
    //
    //		[long] DlpWriteUserInfoReqHdrType.userID		: Not used here - set to
    // zero 		[long] DlpWriteUserInfoReqHdrType.viewerID		: Not used here -
    // set to zero [long] DlpWriteUserInfoReqHdrType.lastSyncPC	: Not used here - set to zero [8byt]
    // DlpWriteUserInfoReqHdrType.lastSyncDate	: Not used here - set to zero 		[long]
    // DlpWriteUserInfoReqHdrType.modFlags		: Bits saying what values are being set
    //		[byte] DlpWriteUserInfoReqHdrType.userNameLen	: Length of user name + NULL
    //
    //		[str ] userName

    char buffer[sizeof(DlpReqHeaderType) + sizeof(DlpTinyArgWrapperType) +
                sizeof(DlpWriteUserInfoReqHdrType) + dlpMaxUserNameSize];

    // Get handy pointers to all of the above.
    DlpReqHeaderType* reqHdr = (DlpReqHeaderType*)buffer;
    DlpTinyArgWrapperType* reqWrapper =
        (DlpTinyArgWrapperType*)(((char*)reqHdr) + sizeof(DlpReqHeaderType));
    DlpWriteUserInfoReqHdrType* reqArgHdr =
        (DlpWriteUserInfoReqHdrType*)(((char*)reqWrapper) + sizeof(DlpTinyArgWrapperType));
    char* reqName = ((char*)reqArgHdr) + sizeof(DlpWriteUserInfoReqHdrType);

    // Fill in request header
    reqHdr->id = dlpWriteUserInfo;
    reqHdr->argc = 1;

    // Fill in the request arg wrapper
    reqWrapper->bID = (UInt8)dlpWriteUserInfoReqArgID;
    reqWrapper->bSize = (UInt8)(sizeof(*reqArgHdr) + userNameLen);

    // Fill in request arg header
    reqArgHdr->modFlags = dlpUserInfoModName;
    reqArgHdr->userNameLen = userNameLen;

    // Copy in the user name.
    strcpy(reqName, userNameP);

    // Build up a session block to hold the command block.
    DlkServerSessionType session;
    memset(&session, 0, sizeof(session));
    session.htalLibRefNum = EmPatchModuleHtal::kMagicRefNum;  // See comments in HtalLibSendReply.
    session.gotCommand = true;
    session.cmdLen = sizeof(buffer);
    session.cmdP = buffer;

    // For simplicity, byteswap here so that we don't have to reparse all
    // that above data in DlkDispatchRequest.

    Canonical(reqHdr->id);
    Canonical(reqHdr->argc);

    Canonical(reqWrapper->bID);
    Canonical(reqWrapper->bSize);

    Canonical(reqArgHdr->modFlags);
    Canonical(reqArgHdr->userNameLen);

    // Patch up cmdP and map in the buffer it points to.

    StMemoryMapper mapper(session.cmdP, session.cmdLen);
    session.cmdP = (void*)(long)EmBankMapped::GetEmulatedAddress(session.cmdP);

    // Finally, install the name.
    DlkDispatchRequest(&session);
}

void SetCurrentDate(void) {
    CEnableFullAccess munge;

    // Get the current date.

    uint32 year, month, day;
    Platform::GetDate(year, month, day);

    // Convert it to days -- and then hourse -- since 1/1/1904

    uint32 rtcHours = ::DateToDays(year, month, day) * 24;

    // Update the "hours adjustment" value to contain the current date.

    emuptr timGlobalsP = EmLowMem_GetGlobal(timGlobalsP);
    EmAliasTimGlobalsType<PAS> timGlobals(timGlobalsP);

    timGlobals.rtcHours = rtcHours;
}

bool GetDatabases(DatabaseInfoList& dbList, uint32 flags) {
    UInt16 cardNo;
    UInt16 numCards;
    UInt16 numDBs;
    Int16 dbIndex;  // UInt16 results in a bug
    LocalID dbID;
    Err err = errNone;
    DatabaseInfo dbInfo;
    Boolean needToAddNewEntry;

    //=======================================================================
    // Cycle through all databases in the ROM and RAM and place them into our list.
    //=======================================================================
    numCards = ::MemNumCards();
    for (cardNo = 0; cardNo < numCards; ++cardNo) {
        numDBs = ::DmNumDatabases(cardNo);

        //---------------------------------------------------------------
        // Loop through databases on this card, DmGetDatabase() returns ROM
        // databases first, followed by RAM databases.
        //---------------------------------------------------------------
        for (dbIndex = 0; dbIndex < numDBs; ++dbIndex) {
            //--------------------------------------------------------
            // Get info on the next database and see if it should be visible.
            //--------------------------------------------------------
            dbID = ::DmGetDatabase(cardNo, dbIndex);
            err = ::DmDatabaseInfo(cardNo, dbID, dbInfo.dbName,            /*nameP*/
                                   &dbInfo.dbAttrs, &dbInfo.version, NULL, /*create date*/
                                   &dbInfo.modDate, NULL,                  /*backup date*/
                                   NULL,                                   /*modNum*/
                                   NULL,                                   /*appInfoID*/
                                   NULL,                                   /*sortInfoID*/
                                   &dbInfo.type, &dbInfo.creator);

            if (err) return false;

            // If it's not supposed to be visible, skip it
            if ((flags & GetDatabaseFlags::kApplicationsOnly) &&
                !::IsVisible(dbInfo.type, dbInfo.creator, dbInfo.dbAttrs)) {
                continue;
            }

            if ((flags & GetDatabaseFlags::kOnlyRamDatabases) && ((dbID & 0x01) == 0)) continue;

            //--------------------------------------------------------------
            // Save info on this database
            //--------------------------------------------------------------
            dbInfo.dbID = dbID;
            dbInfo.cardNo = cardNo;

            //--------------------------------------------------------------
            // If it's an executable, make sure it's the most recent version in our
            // list
            //--------------------------------------------------------------
            needToAddNewEntry = true;
            if ((flags & GetDatabaseFlags::kApplicationsOnly) &&
                IsExecutable(dbInfo.type, dbInfo.creator, dbInfo.dbAttrs)) {
                // Search for database of same type and creator and check version
                DatabaseInfoList::iterator thisIter = dbList.begin();
                while (thisIter != dbList.end()) {
                    if ((*thisIter).type == dbInfo.type && (*thisIter).creator == dbInfo.creator) {
                        // If this new one is a newer or same version than the previous one,
                        // replace the previous entry. Checking for == version allows RAM
                        // executables to override ROM ones.
                        if (dbInfo.version >= (*thisIter).version) {
                            if (flags & GetDatabaseFlags::kGetExtraInfo)
                                ::AppGetExtraInfo(&dbInfo);
                            else
                                strcpy(dbInfo.name, dbInfo.dbName);

                            *thisIter = dbInfo;
                        }

                        // Since there's already an item with this type/creator
                        // already in the list, there's no need to add another one.
                        needToAddNewEntry = false;

                        break;
                    }

                    ++thisIter;
                }
            }

            //--------------------------------------------------------------
            // If we still need to add this entry, do so now.
            //--------------------------------------------------------------
            if (needToAddNewEntry) {
                if (flags & GetDatabaseFlags::kGetExtraInfo)
                    ::AppGetExtraInfo(&dbInfo);
                else
                    strcpy(dbInfo.name, dbInfo.dbName);

                dbList.push_back(dbInfo);
            }
        }  // for (dbIndex = 0; dbIndex < numDBs; dbIndex++)
    }      // for (cardNo = 0; cardNo < MemNumCards(); cardNo++)

    //===========================================================================
    // Sort the list by name
    //===========================================================================
    // Sort the databases by their name.
    sort(dbList.begin(), dbList.end(), AppCompareDataBaseNames);

    return true;
}

bool IsExecutable(const DatabaseInfo& dbInfo) {
    return IsExecutable(dbInfo.type, dbInfo.creator, dbInfo.dbAttrs);
}

string Utf8ToIsolatin1(const string& input, size_t maxLen) {
    size_t len = 0;

    maxLen = min(input.size(), maxLen);
    char* output = new char[maxLen + 1];

    memset(output, 0, maxLen + 1);

    const char* inputChar = input.c_str();

    for (size_t i = 0; i < input.size(); i++) {
        if ((inputChar[i] & 0x80) == 0)
            output[len++] = inputChar[i];
        else if ((inputChar[i] & 0xfc) == 0xc0 && i + 1 < input.size()) {
            output[len++] = ((inputChar[i] & 0x03) << 6) | (inputChar[i + 1] & 0x3f);
            i++;
        } else if ((inputChar[i] & 0xc0) != 0x80)
            output[len++] = '_';

        if (len >= maxLen) break;
    }

    string result = output;
    delete[] output;

    return result;
}

string Isolatin1ToUtf8(const string& input) {
    size_t len = 0;

    char* output = new char[2 * input.size() + 1];
    memset(output, 0, 2 * input.size() + 1);

    const char* inputChar = input.c_str();

    for (size_t i = 0; i < input.size(); i++) {
        if ((inputChar[i] & 0x80) == 0)
            output[len++] = inputChar[i];
        else {
            output[len++] = 0xc0 | ((inputChar[i] >> 6) & 0x03);
            output[len++] = 0x80 | (inputChar[i] & 0x3f);
        }
    }

    string result = output;
    delete[] output;

    return result;
}

void TransformPenCoordinates(int16& x, int16& y) {
    x = 500 - x;
    y = 500 - y;
}

void InstallCalibrationInfo(void) {
    // Open the preferences database.  If the new version of PrefOpenPreferenceDB
    // exists, then call it.  Otherwise, call the old version.	We can't just
    // unconditionally call the old version, as that has a bug in the newer
    // ROMs that causes it to create the database incorrectly if it doesn't
    // already exsist.

    emuptr dbP;
    if (EmLowMem::TrapExists(sysTrapPrefOpenPreferenceDB))
        dbP = ::PrefOpenPreferenceDB(false);
    else
        dbP = ::PrefOpenPreferenceDBV10();

    if (dbP) {
        // Get the calibration information.

        emuptr resourceH = ::DmGetResource(sysResTSysPref, sysResIDSysPrefCalibration);

        // If that information doesn't exist, go about creating it.

        if (!resourceH) {
            resourceH =
                DmNewResource(dbP, sysResTSysPref, sysResIDSysPrefCalibration, 4 * sizeof(UInt16));
        }

        if (resourceH) {
            // Write in the calibration information.  The information has the
            // following format and values:
            //
            //		scaleX	: 256
            //		scaleY	: 256
            //		offsetX :	0
            //		offsetY :	0
            //
            // We encode that data here as a string of bytes to avoid endian problems.

            emuptr resP = ::MemHandleLock(resourceH);

            unsigned char data[] = {1, 0, 1, 0, 0, 0, 0, 0};

            ::DmWrite(resP, 0, data, 4 * sizeof(UInt16));

            ::MemHandleUnlock(resourceH);
            ::DmReleaseResource(resourceH);
        }

        ::DmCloseDatabase(dbP);
    }
}
