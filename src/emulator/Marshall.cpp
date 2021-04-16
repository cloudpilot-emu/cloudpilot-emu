#include "EmPalmStructs.h"
#include "Marshal.h"
#include "ROMStubs.h"

void Marshal::GetPointType(emuptr p, PointType& dest) {
    memset(&dest, 0, sizeof(PointType));

    if (p) {
        EmAliasPointType<PAS> src(p);

        dest.x = src.x;
        dest.y = src.y;
    }
}

void Marshal::PutPointType(emuptr p, const PointType& src) {
    if (p) {
        EmAliasPointType<PAS> dest(p);

        dest.x = src.x;
        dest.y = src.y;
    }
}

void Marshal::GetSysKernelInfoType(emuptr p, SysKernelInfoType& dest) {
    memset(&dest, 0, sizeof(SysKernelInfoType));

    if (p) {
        EmAliasSysKernelInfoType<PAS> src(p);

        dest.selector = src.selector;
        dest.reserved = src.reserved;
        dest.id = src.id;

        switch (dest.selector) {
            case sysKernelInfoSelCurTaskInfo:
            case sysKernelInfoSelTaskInfo:
                dest.param.task.id = src.task.id;
                dest.param.task.nextID = src.task.nextID;
                dest.param.task.tag = src.task.tag;
                dest.param.task.status = src.task.status;
                dest.param.task.timer = src.task.timer;
                dest.param.task.timeSlice = src.task.timeSlice;
                dest.param.task.priority = src.task.priority;
                dest.param.task.attributes = src.task.attributes;
                dest.param.task.pendingCalls = src.task.pendingCalls;
                dest.param.task.senderTaskID = src.task.senderTaskID;
                dest.param.task.msgExchangeID = src.task.msgExchangeID;
                dest.param.task.tcbP = src.task.tcbP;
                dest.param.task.stackP = src.task.stackP;
                dest.param.task.stackStart = src.task.stackStart;
                dest.param.task.stackSize = src.task.stackSize;
                break;

            case sysKernelInfoSelSemaphoreInfo:
                dest.param.semaphore.id = src.semaphore.id;
                dest.param.semaphore.nextID = src.semaphore.nextID;
                dest.param.semaphore.tag = src.semaphore.tag;
                dest.param.semaphore.initValue = src.semaphore.initValue;
                dest.param.semaphore.curValue = src.semaphore.curValue;
                dest.param.semaphore.nestLevel = src.semaphore.nestLevel;
                dest.param.semaphore.ownerID = src.semaphore.ownerID;
                break;

            case sysKernelInfoSelTimerInfo:
                dest.param.timer.id = src.timer.id;
                dest.param.timer.nextID = src.timer.nextID;
                dest.param.timer.tag = src.timer.tag;
                dest.param.timer.ticksLeft = src.timer.ticksLeft;
                dest.param.timer.period = src.timer.period;
                dest.param.timer.proc = src.timer.proc;
                break;

            default:
                EmAssert(false);
        }
    }
}

void Marshal::PutSysKernelInfoType(emuptr p, const SysKernelInfoType& src) {
    if (p) {
        EmAliasSysKernelInfoType<PAS> dest(p);

        dest.selector = src.selector;
        dest.reserved = src.reserved;
        dest.id = src.id;

        switch (dest.selector) {
            case sysKernelInfoSelCurTaskInfo:
            case sysKernelInfoSelTaskInfo:
                dest.task.id = src.param.task.id;
                dest.task.nextID = src.param.task.nextID;
                dest.task.tag = src.param.task.tag;
                dest.task.status = src.param.task.status;
                dest.task.timer = src.param.task.timer;
                dest.task.timeSlice = src.param.task.timeSlice;
                dest.task.priority = src.param.task.priority;
                dest.task.attributes = src.param.task.attributes;
                dest.task.pendingCalls = src.param.task.pendingCalls;
                dest.task.senderTaskID = src.param.task.senderTaskID;
                dest.task.msgExchangeID = src.param.task.msgExchangeID;
                dest.task.tcbP = src.param.task.tcbP;
                dest.task.stackP = src.param.task.stackP;
                dest.task.stackStart = src.param.task.stackStart;
                dest.task.stackSize = src.param.task.stackSize;
                break;

            case sysKernelInfoSelSemaphoreInfo:
                dest.semaphore.id = src.param.semaphore.id;
                dest.semaphore.nextID = src.param.semaphore.nextID;
                dest.semaphore.tag = src.param.semaphore.tag;
                dest.semaphore.initValue = src.param.semaphore.initValue;
                dest.semaphore.curValue = src.param.semaphore.curValue;
                dest.semaphore.nestLevel = src.param.semaphore.nestLevel;
                dest.semaphore.ownerID = src.param.semaphore.ownerID;
                break;

            case sysKernelInfoSelTimerInfo:
                dest.timer.id = src.param.timer.id;
                dest.timer.nextID = src.param.timer.nextID;
                dest.timer.tag = src.param.timer.tag;
                dest.timer.ticksLeft = src.param.timer.ticksLeft;
                dest.timer.period = src.param.timer.period;
                dest.timer.proc = src.param.timer.proc;
                break;

            default:
                EmAssert(false);
        }
    }
}

void Marshal::PutDlkServerSessionType(emuptr p, const DlkServerSessionType& src) {
    if (p) {
        EmAliasDlkServerSessionType<PAS> dest(p);

        dest.htalLibRefNum = src.htalLibRefNum;
        dest.maxHtalXferSize = src.maxHtalXferSize;
        dest.eventProcP = (emuptr)(long)src.eventProcP;
        dest.eventRef = src.eventRef;
        dest.canProcP = (emuptr)(long)src.canProcP;
        dest.canRef = src.canRef;
        dest.condFilterH = (emuptr)(long)src.condFilterH;
        dest.dlkDBID = src.dlkDBID;
        dest.reserved1 = src.reserved1;
        dest.dbR = (emuptr)(long)src.dbR;
        dest.cardNo = src.cardNo;
        dest.dbCreator = src.dbCreator;
        //	dest.dbName				= src.dbName;
        dest.dbOpenMode = src.dbOpenMode;
        dest.created = src.created;
        dest.isResDB = src.isResDB;
        dest.ramBased = src.ramBased;
        dest.readOnly = src.readOnly;
        dest.dbLocalID = src.dbLocalID;
        dest.initialModNum = src.initialModNum;
        dest.curRecIndex = src.curRecIndex;
        //	dest.creatorList		= src.creatorList;
        dest.syncState = src.syncState;
        dest.complete = src.complete;
        dest.conduitOpened = src.conduitOpened;
        dest.logCleared = src.logCleared;
        dest.resetPending = src.resetPending;
        dest.gotCommand = src.gotCommand;
        dest.cmdTID = src.cmdTID;
        dest.reserved2 = src.reserved2;
        dest.cmdLen = src.cmdLen;
        dest.cmdP = (emuptr)(long)src.cmdP;
        dest.cmdH = (emuptr)(long)src.cmdH;
        dest.wStateFlags = src.wStateFlags;
        //	dest.dbSearchState		= src.dbSearchState;
    }
}

void Marshal::GetEventType(emuptr p, EventType& dest) {
    memset(&dest, 0, sizeof(EventType));

    if (p) {
        EmAliasEventType<PAS> src(p);

        dest.eType = src.eType;
        dest.penDown = src.penDown;
        dest.screenX = src.screenX;
        dest.screenY = src.screenY;

        switch (dest.eType) {
            case nilEvent:
                break;

            case penDownEvent:
                dest.data.penUp.start.x = src.data.penUp.start.x;
                dest.data.penUp.start.y = src.data.penUp.start.y;
                dest.data.penUp.end.x = src.data.penUp.end.x;
                dest.data.penUp.end.y = src.data.penUp.end.y;
                break;

            case penUpEvent:
                dest.data.penUp.start.x = src.data.penUp.start.x;
                dest.data.penUp.start.y = src.data.penUp.start.y;
                dest.data.penUp.end.x = src.data.penUp.end.x;
                dest.data.penUp.end.y = src.data.penUp.end.y;
                break;

            case penMoveEvent:
                dest.data.penUp.start.x = src.data.penUp.start.x;
                dest.data.penUp.start.y = src.data.penUp.start.y;
                dest.data.penUp.end.x = src.data.penUp.end.x;
                dest.data.penUp.end.y = src.data.penUp.end.y;
                break;

            case keyDownEvent:
                dest.data.keyDown.chr = src.data.keyDown.chr;
                dest.data.keyDown.keyCode = src.data.keyDown.keyCode;
                dest.data.keyDown.modifiers = src.data.keyDown.modifiers;
                break;

            case winEnterEvent:
                dest.data.winEnter.enterWindow = (WinHandle)(long)src.data.winEnter.enterWindow;
                dest.data.winEnter.exitWindow = (WinHandle)(long)src.data.winEnter.exitWindow;
                break;

            case winExitEvent:
                dest.data.winExit.enterWindow = (WinHandle)(long)src.data.winExit.enterWindow;
                dest.data.winExit.exitWindow = (WinHandle)(long)src.data.winExit.exitWindow;
                break;

            case ctlEnterEvent:
                dest.data.ctlEnter.controlID = src.data.ctlEnter.controlID;
                dest.data.ctlEnter.pControl = (struct ControlType*)(long)src.data.ctlEnter.pControl;
                break;

            case ctlExitEvent:
                dest.data.ctlExit.controlID = src.data.ctlExit.controlID;
                dest.data.ctlExit.pControl = (struct ControlType*)(long)src.data.ctlExit.pControl;
                break;

            case ctlSelectEvent:
                dest.data.ctlSelect.controlID = src.data.ctlSelect.controlID;
                dest.data.ctlSelect.pControl =
                    (struct ControlType*)(long)src.data.ctlSelect.pControl;
                dest.data.ctlSelect.on = src.data.ctlSelect.on;
                break;

            case ctlRepeatEvent:
                dest.data.ctlRepeat.controlID = src.data.ctlRepeat.controlID;
                dest.data.ctlRepeat.pControl =
                    (struct ControlType*)(long)src.data.ctlRepeat.pControl;
                dest.data.ctlRepeat.time = src.data.ctlRepeat.time;
                break;

            case lstEnterEvent:
                dest.data.lstEnter.listID = src.data.lstEnter.listID;
                dest.data.lstEnter.pList = (struct ListType*)(long)src.data.lstEnter.pList;
                dest.data.lstEnter.selection = src.data.lstEnter.selection;
                break;

            case lstSelectEvent:
                dest.data.lstSelect.listID = src.data.lstSelect.listID;
                dest.data.lstSelect.pList = (struct ListType*)(long)src.data.lstSelect.pList;
                dest.data.lstSelect.selection = src.data.lstSelect.selection;
                break;

            case lstExitEvent:
                dest.data.lstExit.listID = src.data.lstExit.listID;
                dest.data.lstExit.pList = (struct ListType*)(long)src.data.lstExit.pList;
                break;

            case popSelectEvent:
                dest.data.popSelect.controlID = src.data.popSelect.controlID;
                dest.data.popSelect.controlP =
                    (struct ControlType*)(long)src.data.popSelect.controlP;
                dest.data.popSelect.listID = src.data.popSelect.listID;
                dest.data.popSelect.listP = (struct ListType*)(long)src.data.popSelect.listP;
                dest.data.popSelect.selection = src.data.popSelect.selection;
                dest.data.popSelect.priorSelection = src.data.popSelect.priorSelection;
                break;

            case fldEnterEvent:
                dest.data.fldEnter.fieldID = src.data.fldEnter.fieldID;
                dest.data.fldEnter.pField = (struct FieldType*)(long)src.data.fldEnter.pField;
                break;

            case fldHeightChangedEvent:
                dest.data.fldHeightChanged.fieldID = src.data.fldHeightChanged.fieldID;
                dest.data.fldHeightChanged.pField =
                    (struct FieldType*)(long)src.data.fldHeightChanged.pField;
                dest.data.fldHeightChanged.newHeight = src.data.fldHeightChanged.newHeight;
                dest.data.fldHeightChanged.currentPos = src.data.fldHeightChanged.currentPos;
                break;

            case fldChangedEvent:
                dest.data.fldChanged.fieldID = src.data.fldChanged.fieldID;
                dest.data.fldChanged.pField = (struct FieldType*)(long)src.data.fldChanged.pField;
                break;

            case tblEnterEvent:
                dest.data.tblEnter.tableID = src.data.tblEnter.tableID;
                dest.data.tblEnter.pTable = (struct TableType*)(long)src.data.tblEnter.pTable;
                dest.data.tblEnter.row = src.data.tblEnter.row;
                dest.data.tblEnter.column = src.data.tblEnter.column;
                break;

            case tblSelectEvent:
                dest.data.tblEnter.tableID = src.data.tblEnter.tableID;
                dest.data.tblEnter.pTable = (struct TableType*)(long)src.data.tblEnter.pTable;
                dest.data.tblEnter.row = src.data.tblEnter.row;
                dest.data.tblEnter.column = src.data.tblEnter.column;
                break;

            case daySelectEvent:
                dest.data.daySelect.pSelector =
                    (struct DaySelectorType*)(long)src.data.daySelect.pSelector;
                dest.data.daySelect.selection = src.data.daySelect.selection;
                dest.data.daySelect.useThisDate = src.data.daySelect.useThisDate;
                break;

            case menuEvent:
                dest.data.menu.itemID = src.data.menu.itemID;
                break;

            case appStopEvent:
                break;

            case frmLoadEvent:
                dest.data.frmLoad.formID = src.data.frmLoad.formID;
                break;

            case frmOpenEvent:
                dest.data.frmOpen.formID = src.data.frmOpen.formID;
                break;

            case frmGotoEvent:
                dest.data.frmGoto.formID = src.data.frmGoto.formID;
                dest.data.frmGoto.recordNum = src.data.frmGoto.recordNum;
                dest.data.frmGoto.matchPos = src.data.frmGoto.matchPos;
                dest.data.frmGoto.matchLen = src.data.frmGoto.matchLen;
                dest.data.frmGoto.matchFieldNum = src.data.frmGoto.matchFieldNum;
                dest.data.frmGoto.matchCustom = src.data.frmGoto.matchCustom;
                break;

            case frmUpdateEvent:
                dest.data.frmUpdate.formID = src.data.frmUpdate.formID;
                dest.data.frmUpdate.updateCode = src.data.frmUpdate.updateCode;
                break;

            case frmSaveEvent:
                break;

            case frmCloseEvent:
                dest.data.frmClose.formID = src.data.frmClose.formID;
                break;

            case frmTitleEnterEvent:
                dest.data.frmTitleEnter.formID = src.data.frmTitleEnter.formID;
                break;

            case frmTitleSelectEvent:
                dest.data.frmTitleSelect.formID = src.data.frmTitleSelect.formID;
                break;

            case tblExitEvent:
                dest.data.tblExit.tableID = src.data.tblExit.tableID;
                dest.data.tblExit.pTable = (struct TableType*)(long)src.data.tblExit.pTable;
                dest.data.tblExit.row = src.data.tblExit.row;
                dest.data.tblExit.column = src.data.tblExit.column;
                break;

            case sclEnterEvent:
                dest.data.sclEnter.scrollBarID = src.data.sclEnter.scrollBarID;
                dest.data.sclEnter.pScrollBar =
                    (struct ScrollBarType*)(long)src.data.sclEnter.pScrollBar;
                break;

            case sclExitEvent:
                dest.data.sclExit.scrollBarID = src.data.sclExit.scrollBarID;
                dest.data.sclExit.pScrollBar =
                    (struct ScrollBarType*)(long)src.data.sclExit.pScrollBar;
                dest.data.sclExit.value = src.data.sclExit.value;
                dest.data.sclExit.newValue = src.data.sclExit.newValue;
                break;

            case sclRepeatEvent:
                dest.data.sclRepeat.scrollBarID = src.data.sclRepeat.scrollBarID;
                dest.data.sclRepeat.pScrollBar =
                    (struct ScrollBarType*)(long)src.data.sclRepeat.pScrollBar;
                dest.data.sclRepeat.value = src.data.sclRepeat.value;
                dest.data.sclRepeat.newValue = src.data.sclRepeat.newValue;
                dest.data.sclRepeat.time = src.data.sclRepeat.time;
                break;

            case tsmConfirmEvent:
                dest.data.tsmConfirm.yomiText = (Char*)(long)src.data.tsmConfirm.yomiText;
                dest.data.tsmConfirm.formID = src.data.tsmConfirm.formID;
                break;

            case tsmFepButtonEvent:
                dest.data.tsmFepButton.buttonID = src.data.tsmFepButton.buttonID;
                break;

            case tsmFepModeEvent:
                dest.data.tsmFepMode.mode = src.data.tsmFepMode.mode;
                break;

            case attnIndicatorEnterEvent:
                dest.data.attnIndicatorEnter.formID = src.data.attnIndicatorEnter.formID;
                break;

            case attnIndicatorSelectEvent:
                dest.data.attnIndicatorSelect.formID = src.data.attnIndicatorSelect.formID;
                break;

            case menuCmdBarOpenEvent:
                dest.data.menuCmdBarOpen.preventFieldButtons =
                    src.data.menuCmdBarOpen.preventFieldButtons;
                dest.data.menuCmdBarOpen.reserved = src.data.menuCmdBarOpen.reserved;
                break;

            case menuOpenEvent:
                dest.data.menuOpen.menuRscID = src.data.menuOpen.menuRscID;
                dest.data.menuOpen.cause = src.data.menuOpen.cause;
                break;

            case menuCloseEvent:
                // Doesn't appear to be used.
                break;

            case frmGadgetEnterEvent:
                dest.data.gadgetEnter.gadgetID = src.data.gadgetEnter.gadgetID;
                dest.data.gadgetEnter.gadgetP =
                    (struct FormGadgetType*)(long)src.data.gadgetEnter.gadgetP;
                break;

            case frmGadgetMiscEvent:
                dest.data.gadgetMisc.gadgetID = src.data.gadgetMisc.gadgetID;
                dest.data.gadgetMisc.gadgetP =
                    (struct FormGadgetType*)(long)src.data.gadgetMisc.gadgetP;
                dest.data.gadgetMisc.selector = src.data.gadgetMisc.selector;
                dest.data.gadgetMisc.dataP = (void*)(long)src.data.gadgetMisc.dataP;
                break;

            default:
                if ((dest.eType >= firstINetLibEvent && dest.eType < firstWebLibEvent + 0x100) ||
                    dest.eType >= firstUserEvent) {
                    // We don't know what's in here, so let's just blockmove it.

                    EmMem_memcpy((void*)dest.data.generic.datum, src.data.generic.datum.GetPtr(),
                                 16);
                } else {
                    EmAssert(false);
                }
        }
    }
}

void Marshal::GetNetIOParamType(emuptr p, NetIOParamType& dest) {
    memset(&dest, 0, sizeof(NetIOParamType));

    if (p) {
        EmAliasNetIOParamType<PAS> src(p);

        // Copy the simple fields.

        dest.addrLen = src.addrLen;
        dest.iovLen = src.iovLen;
        dest.accessRightsLen = src.accessRightsLen;

        // Copy the address field.

        dest.addrP = (UInt8*)GetBuffer(src.addrP, src.addrLen);
        if (dest.addrP) {
            // The "family" field needs to be stored in HBO, not NBO.
            // The following sequence will assure that.

            EmAliasNetSocketAddrType<PAS> addr(src.addrP);

            ((NetSocketAddrType*)(dest.addrP))->family = addr.family;
        }

        // Copy the access rights field.

        dest.accessRights = (UInt8*)GetBuffer(src.accessRights, dest.accessRightsLen);

        // Copy the i/o buffers.

        dest.iov = (NetIOVecPtr)Platform::AllocateMemory(dest.iovLen * sizeof(NetIOVecType));
        emuptr iovP = src.iov;

        for (UInt16 ii = 0; ii < dest.iovLen; ++ii) {
            EmAliasNetIOVecType<PAS> iov(iovP);

            dest.iov[ii].bufLen = iov.bufLen;
            dest.iov[ii].bufP = (UInt8*)GetBuffer(iov.bufP, iov.bufLen);

            iovP += iov.GetSize();
        }
    }
}

void Marshal::GetNetSocketAddrType(emuptr p, NetSocketAddrType& d) {
    memset(&d, 0, sizeof(NetSocketAddrType));

    if (p) {
        EmAliasNetSocketAddrType<PAS> sockAddr(p);

        switch (sockAddr.family) {
            case netSocketAddrRaw: {
                EmAliasNetSocketAddrRawType<PAS> src(p);
                NetSocketAddrRawType& dest = (NetSocketAddrRawType&)d;

                dest.family = src.family;          // In HBO
                dest.ifInstance = src.ifInstance;  // Unspecified order
                dest.ifCreator = src.ifCreator;    // Unspecified order

                break;
            }

            case netSocketAddrINET: {
                EmAliasNetSocketAddrINType<PAS> src(p);
                NetSocketAddrINType& dest = (NetSocketAddrINType&)d;

                dest.family = src.family;                       // In HBO
                dest.port = htons(NetNToHS((UInt16)src.port));  // In NBO
                dest.addr = htonl(NetNToHL((UInt32)src.addr));  // In NBO

                break;
            }

            default: {
                // Do the best we can...
                EmAliasNetSocketAddrType<PAS> src(p);
                NetSocketAddrType& dest = (NetSocketAddrType&)d;

                dest.family = src.family;  // In HBO

                EmMem_memcpy((void*)&dest.data[0], src.data.GetPtr(), 14);
                break;
            }
        }
    }
}

void* Marshal::GetBuffer(emuptr p, long len) {
    void* result = NULL;

    if (p) {
        result = Platform::AllocateMemory(len);

        if (result) {
            EmMem_memcpy(result, p, len);
        }
    }

    return result;
}
