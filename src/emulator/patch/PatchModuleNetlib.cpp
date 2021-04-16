#include "PatchModuleNetlib.h"

#include "EmCommon.h"
#include "Logging.h"
#include "Marshal.h"

#define LOGGING 1

#ifdef LOGGING
    #define PRINTF logging::printf
#else
    #define PRINTF(...) ;
#endif

namespace {
    CallROMType HeadpatchNetLibOpen(void) {
        PRINTF("NetLibOpen");

        //	Err	NetLibOpen (UInt16 libRefNum, UInt16 *netIFErrsP)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 *netIFErrsP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_REF(UInt16, netIFErrsP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibClose(void) {
        PRINTF("NetLibClose");

        //	Err NetLibClose (UInt16 libRefNum, UInt16 immediate)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 immediate");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(UInt16, immediate);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSleep(void) {
        PRINTF("NetLibSleep");

        //	Err NetLibSleep (UInt16 libRefNum)

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibWake(void) {
        PRINTF("NetLibWake");

        //	Err NetLibWake (UInt16 libRefNum)

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibAddrINToA(void) {
        PRINTF("NetLibAddrINToA");

        //	Char * NetLibAddrINToA(UInt16 libRefNum, NetIPAddr inet, Char *spaceP)

        CALLED_SETUP("Char*", "UInt16 libRefNum, NetIPAddr inet, Char *spaceP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibAddrAToIN(void) {
        PRINTF("NetLibAddrAToIN");

        //	NetIPAddr NetLibAddrAToIN(UInt16 libRefNum, Char *a)

        CALLED_SETUP("NetIPAddr", "UInt16 libRefNum, Char *a");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketOpen(void) {
        PRINTF("NetLibSocketOpen");

        //	NetSocketRef	NetLibSocketOpen(UInt16 libRefNum, NetSocketAddrEnum domain,
        //						NetSocketTypeEnum type, Int16 protocol,
        // Int32 timeout, 						Err *errP)

        CALLED_SETUP("NetSocketRef",
                     "UInt16 libRefNum, NetSocketAddrEnum domain, "
                     "NetSocketTypeEnum type, Int16 protocol, Int32 timeout, "
                     "Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketAddrEnum, domain);
        CALLED_GET_PARAM_VAL(NetSocketTypeEnum, type);
        CALLED_GET_PARAM_VAL(Int16, protocol);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketClose(void) {
        PRINTF("NetLibSocketClose");

        //	Int16 NetLibSocketClose(UInt16 libRefNum, NetSocketRef socket, Int32 timeout,
        //						Err *errP)

        CALLED_SETUP("Int16", "UInt16 libRefNum, NetSocketRef socket, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketOptionSet(void) {
        PRINTF("NetLibSocketOptionSet");

        //	Int16 NetLibSocketOptionSet(UInt16 libRefNum, NetSocketRef socket,
        //						UInt16 /*NetSocketOptLevelEnum*/ level,
        // UInt16
        ///*NetSocketOptEnum*/ option, 						void *optValueP,
        /// UInt16 optValueLen, 						Int32 timeout, Err
        //*errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "UInt16 level, UInt16 option, "
                     "void *optValueP, UInt16 optValueLen,"
                     "Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(UInt16, level);
        CALLED_GET_PARAM_VAL(UInt16, option);
        CALLED_GET_PARAM_VAL(UInt16, optValueLen);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        CALLED_GET_PARAM_PTR(void, optValueP, optValueLen, Marshal::kInput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketOptionGet(void) {
        PRINTF("NetLibSocketOptionGet");

        //	Int16 NetLibSocketOptionGet(UInt16 libRefNum, NetSocketRef socket,
        //						UInt16 /*NetSocketOptLevelEnum*/ level,
        // UInt16
        ///*NetSocketOptEnum*/ option, 						void *optValueP,
        /// UInt16 *optValueLenP, 						Int32 timeout, Err
        //*errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "UInt16 level, UInt16 option,"
                     "void *optValueP, UInt16 *optValueLenP,"
                     "Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(UInt16, level);
        CALLED_GET_PARAM_VAL(UInt16, option);
        CALLED_GET_PARAM_REF(UInt16, optValueLenP, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        CALLED_GET_PARAM_PTR(void, optValueP, *optValueLenP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketBind(void) {
        PRINTF("NetLibSocketBind");

        //	Int16 NetLibSocketBind(UInt16 libRefNum, NetSocketRef socket,
        //						NetSocketAddrType *sockAddrP, Int16 addrLen,
        // Int32 timeout, 						Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "NetSocketAddrType *sockAddrP, Int16 addrLen, Int32 timeout, "
                     "Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_REF(NetSocketAddrType, sockAddrP, Marshal::kInput);
        CALLED_GET_PARAM_VAL(Int16, addrLen);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketConnect(void) {
        PRINTF("NetLibSocketConnect");

        //	Int16 NetLibSocketConnect(UInt16 libRefNum, NetSocketRef socket,
        //						NetSocketAddrType *sockAddrP, Int16 addrLen,
        // Int32 timeout, 						Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "NetSocketAddrType *sockAddrP, Int16 addrLen, Int32 timeout, "
                     "Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_REF(NetSocketAddrType, sockAddrP, Marshal::kInput);
        CALLED_GET_PARAM_VAL(Int16, addrLen);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketListen(void) {
        PRINTF("NetLibSocketListen");

        //	Int16 NetLibSocketListen(UInt16 libRefNum, NetSocketRef socket,
        //						UInt16	queueLen, Int32 timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "UInt16	queueLen, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(UInt16, queueLen);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketAccept(void) {
        PRINTF("NetLibSocketAccept");

        //	Int16 NetLibSocketAccept(UInt16 libRefNum, NetSocketRef socket,
        //						NetSocketAddrType *sockAddrP, Int16
        //*addrLenP, Int32 timeout, 						Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "NetSocketAddrType *sockAddrP, Int16 *addrLenP, Int32 timeout,"
                     "Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_REF(NetSocketAddrType, sockAddrP, Marshal::kOutput);
        CALLED_GET_PARAM_REF(Int16, addrLenP, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketShutdown(void) {
        PRINTF("NetLibSocketShutdown");

        //	Int16 NetLibSocketShutdown(UInt16 libRefNum, NetSocketRef socket,
        //						Int16 /*NetSocketDirEnum*/ direction, Int32
        // timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket, "
                     "Int16 direction, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(Int16, direction);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSendPB(void) {
        PRINTF("NetLibSendPB");

        // Int16 NetLibSendPB(UInt16 libRefNum, NetSocketRef socket,
        //						NetIOParamType *pbP, UInt16 flags, Int32
        // timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "NetIOParamType *pbP, UInt16 flags, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_REF(NetIOParamType, pbP, Marshal::kInput);
        CALLED_GET_PARAM_VAL(UInt16, flags);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSend(void) {
        PRINTF("NetLibSend");

        //	Int16 NetLibSend(UInt16 libRefNum, NetSocketRef socket,
        //						void *bufP, UInt16 bufLen, UInt16 flags,
        //						void *toAddrP, UInt16 toLen, Int32 timeout,
        // Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "void *bufP, UInt16 bufLen, UInt16 flags,"
                     "void *toAddrP, UInt16 toLen, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(UInt16, bufLen);
        CALLED_GET_PARAM_VAL(UInt16, flags);
        CALLED_GET_PARAM_REF(NetSocketAddrType, toAddrP, Marshal::kInput);
        CALLED_GET_PARAM_VAL(UInt16, toLen);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        CALLED_GET_PARAM_PTR(void, bufP, bufLen, Marshal::kInput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibReceivePB(void) {
        PRINTF("NetLibReceivePB");

        //	Int16 NetLibReceivePB(UInt16 libRefNum, NetSocketRef socket,
        //						NetIOParamType *pbP, UInt16 flags, Int32
        // timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "NetIOParamType *pbP, UInt16 flags, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_REF(NetIOParamType, pbP, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(UInt16, flags);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibReceive(void) {
        PRINTF("NetLibReceive");

        //	Int16 NetLibReceive(UInt16 libRefNum, NetSocketRef socket,
        //						void *bufP, UInt16 bufLen, UInt16 flags,
        //						void *fromAddrP, UInt16 *fromLenP, Int32
        // timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "void *bufP, UInt16 bufLen, UInt16 flags, "
                     "void *fromAddrP, UInt16 *fromLenP, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(UInt16, bufLen);
        CALLED_GET_PARAM_VAL(UInt16, flags);
        CALLED_GET_PARAM_REF(NetSocketAddrType, fromAddrP, Marshal::kOutput);
        CALLED_GET_PARAM_REF(UInt16, fromLenP, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        CALLED_GET_PARAM_PTR(void, bufP, bufLen, Marshal::kInOut);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibDmReceive(void) {
        PRINTF("NetLibDmReceive");

        //	Int16 NetLibDmReceive(UInt16 libRefNum, NetSocketRef socket,
        //						void *recordP, UInt32 recordOffset, UInt16
        // rcvLen,
        // UInt16 flags, 						void *fromAddrP, UInt16
        // *fromLenP, Int32 timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "void *recordP, UInt32 recordOffset, UInt16 rcvLen, UInt16 flags, "
                     "void *fromAddrP, UInt16 *fromLenP, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(UInt32, recordOffset);
        CALLED_GET_PARAM_VAL(UInt16, rcvLen);
        CALLED_GET_PARAM_VAL(UInt16, flags);
        CALLED_GET_PARAM_REF(NetSocketAddrType, fromAddrP, Marshal::kOutput);
        CALLED_GET_PARAM_REF(UInt16, fromLenP, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        CALLED_GET_PARAM_PTR(void, recordP, rcvLen + recordOffset, Marshal::kInOut);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSelect(void) {
        PRINTF("NetLibSelect");

        //	Int16 NetLibSelect(UInt16 libRefNum, UInt16 width, NetFDSetType *readFDs,
        //						NetFDSetType *writeFDs, NetFDSetType
        //*exceptFDs, 						Int32	timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, UInt16 width, NetFDSetType *readFDs, "
                     "NetFDSetType *writeFDs, NetFDSetType *exceptFDs,"
                     "Int32	timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(UInt16, width);
        CALLED_GET_PARAM_REF(NetFDSetType, readFDs, Marshal::kInOut);
        CALLED_GET_PARAM_REF(NetFDSetType, writeFDs, Marshal::kInOut);
        CALLED_GET_PARAM_REF(NetFDSetType, exceptFDs, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibMaster(void) {
        PRINTF("NetLibMaster");

        //	Err NetLibMaster(UInt16 libRefNum, UInt16 cmd, NetMasterPBPtr pbP,
        //					Int32 timeout)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 cmd, NetMasterPBPtr pbP,"
                     "Int32 timeout");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibGetHostByName(void) {
        PRINTF("NetLibGetHostByName");

        //	NetHostInfoPtr NetLibGetHostByName(UInt16 libRefNum, Char *nameP,
        //						NetHostInfoBufPtr bufP, Int32	timeout, Err
        //*errP)

        CALLED_SETUP("NetHostInfoPtr",
                     "UInt16 libRefNum, Char *nameP, "
                     "NetHostInfoBufPtr bufP, Int32	timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_STR(Char, nameP);
        CALLED_GET_PARAM_REF(NetHostInfoBufType, bufP, Marshal::kOutput);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSettingGet(void) {
        PRINTF("NetLibSettingGet");

        //	Err NetLibSettingGet(UInt16 libRefNum,
        //						UInt16 /*NetSettingEnum*/ setting, void
        //*valueP, UInt16 *valueLenP)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 setting, void *valueP, UInt16 *valueLenP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSettingSet(void) {
        PRINTF("NetLibSettingSet");

        //	Err NetLibSettingSet(UInt16 libRefNum,
        //						UInt16 /*NetSettingEnum*/ setting, void
        //*valueP, UInt16 valueLen)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 setting, void *valueP, UInt16 valueLen");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFAttach(void) {
        PRINTF("NetLibIFAttach");

        //	Err NetLibIFAttach(UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,
        //						Int32 timeout)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance, Int32 timeout");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFDetach(void) {
        PRINTF("NetLibIFDetach");

        //	Err NetLibIFDetach(UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,
        //						Int32 timeout)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,"
                     "Int32 timeout");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFGet(void) {
        PRINTF("NetLibIFGet");

        //	Err NetLibIFGet(UInt16 libRefNum, UInt16 index, UInt32 *ifCreatorP,
        //							UInt16 *ifInstanceP)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 index, UInt32 *ifCreatorP, "
                     "UInt16 *ifInstanceP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFSettingGet(void) {
        PRINTF("NetLibIFSettingGet");

        //	Err NetLibIFSettingGet(UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,
        //						UInt16 /*NetIFSettingEnum*/ setting, void
        //*valueP, UInt16 *valueLenP)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,"
                     "UInt16 setting, void *valueP, UInt16 *valueLenP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFSettingSet(void) {
        PRINTF("NetLibIFSettingSet");

        //	Err NetLibIFSettingSet(UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,
        //						UInt16 /*NetIFSettingEnum*/ setting, void
        //*valueP, UInt16 valueLen)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,"
                     "UInt16 setting, void *valueP, UInt16 valueLen");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFUp(void) {
        PRINTF("NetLibIFUp");

        //	Err NetLibIFUp(UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFDown(void) {
        PRINTF("NetLibIFDown");

        //	Err NetLibIFDown(UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,
        //						Int32 timeout)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance, Int32 timeout");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibGetHostByAddr(void) {
        PRINTF("NetLibGetHostByAddr");

        //	NetHostInfoPtr NetLibGetHostByAddr(UInt16 libRefNum, UInt8 *addrP, UInt16 len,
        // UInt16 type, 						NetHostInfoBufPtr bufP,
        // Int32 timeout, Err *errP)

        CALLED_SETUP("NetHostInfoPtr",
                     "UInt16 libRefNum, UInt8 *addrP, UInt16 len, UInt16 type,"
                     "NetHostInfoBufPtr bufP, Int32	timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(UInt16, len);
        CALLED_GET_PARAM_PTR(UInt8, addrP, len, Marshal::kInput);
        CALLED_GET_PARAM_VAL(UInt16, type);
        CALLED_GET_PARAM_REF(NetHostInfoBufType, bufP, Marshal::kOutput);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibGetServByName(void) {
        PRINTF("NetLibGetServByName");

        //	NetServInfoPtr	NetLibGetServByName(UInt16 libRefNum, const Char *servNameP,
        //						const Char *protoNameP,  NetServInfoBufPtr
        // bufP, 						Int32	timeout, Err *errP)

        CALLED_SETUP("NetServInfoPtr",
                     "UInt16 libRefNum, const Char *servNameP, "
                     "const Char *protoNameP,  NetServInfoBufPtr bufP, "
                     "Int32	timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_STR(Char, servNameP);
        CALLED_GET_PARAM_STR(Char, protoNameP);
        CALLED_GET_PARAM_REF(NetServInfoBufType, bufP, Marshal::kOutput);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketAddr(void) {
        PRINTF("NetLibSocketAddr");

        //	Int16 NetLibSocketAddr(UInt16 libRefNum, NetSocketRef socket,
        //						NetSocketAddrType *locAddrP, Int16
        //*locAddrLenP, 						NetSocketAddrType *remAddrP,
        // Int16 *remAddrLenP, 						Int32 timeout, Err *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "NetSocketAddrType *locAddrP, Int16 *locAddrLenP, "
                     "NetSocketAddrType *remAddrP, Int16 *remAddrLenP, "
                     "Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_REF(NetSocketAddrType, locAddrP, Marshal::kOutput);
        CALLED_GET_PARAM_REF(Int16, locAddrLenP, Marshal::kInOut);
        CALLED_GET_PARAM_REF(NetSocketAddrType, remAddrP, Marshal::kOutput);
        CALLED_GET_PARAM_REF(Int16, remAddrLenP, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibFinishCloseWait(void) {
        PRINTF("NetLibFinishCloseWait");

        //	Err NetLibFinishCloseWait(UInt16 libRefNum)

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibGetMailExchangeByName(void) {
        PRINTF("NetLibGetMailExchangeByName");

        //	Int16 NetLibGetMailExchangeByName(UInt16 libRefNum, Char *mailNameP,
        //						UInt16 maxEntries,
        //						Char hostNames[][netDNSMaxDomainName+1],
        // UInt16 priorities[], 						Int32	timeout, Err
        // *errP)

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, Char *mailNameP, "
                     "UInt16 maxEntries, "
                     "Char** hostNames, UInt16* priorities, "
                     "Int32	timeout, Err *errP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibOpenCount(void) {
        PRINTF("NetLibOpenCount");

        //	Err NetLibOpenCount (UInt16 libRefNum, UInt16 *countP)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 *countP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_REF(UInt16, countP, Marshal::kOutput);
        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibTracePrintF(void) {
        PRINTF("NetLibTracePrintF");

        //	Err NetLibTracePrintF(UInt16 libRefNum, Char *formatStr, ...)

        CALLED_SETUP("Err", "UInt16 libRefNum, Char *formatStr");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibTracePutS(void) {
        PRINTF("NetLibTracePutS");

        // Err NetLibTracePutS(UInt16 libRefNum, Char *strP)

        CALLED_SETUP("Err", "UInt16 libRefNum, Char *strP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibOpenIfCloseWait(void) {
        PRINTF("NetLibOpenIfCloseWait");

        //	Err NetLibOpenIfCloseWait(UInt16 libRefNum)

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibHandlePowerOff(void) {
        PRINTF("NetLibHandlePowerOff");

        //	Err NetLibHandlePowerOff (UInt16 libRefNum, SysEventType *eventP)

        CALLED_SETUP("Err", "UInt16 libRefNum, SysEventType *eventP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_REF(EventType, eventP, Marshal::kInput);
        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConnectionRefresh(void) {
        PRINTF("NetLibConnectionRefresh");

        //	Err NetLibConnectionRefresh(UInt16 libRefNum, Boolean refresh,
        //						UInt8 *allInterfacesUpP, UInt16 *netIFErrP)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, Boolean refresh, "
                     "UInt8 *allInterfacesUpP, UInt16 *netIFErrP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(Boolean, refresh);
        CALLED_GET_PARAM_REF(Boolean, allInterfacesUpP, Marshal::kOutput);
        CALLED_GET_PARAM_REF(UInt16, netIFErrP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibOpenConfig(void) {
        PRINTF("NetLibOpenConfig");

        //	Err NetLibOpenConfig( UInt16 libRefNum, UInt16 configIndex, UInt32 openFlags,
        //						UInt16 *netIFErrP)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 configIndex, UInt32 openFlags,"
                     "UInt16 *netIFErrP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(UInt16, configIndex);
        CALLED_GET_PARAM_VAL(UInt32, openFlags);
        CALLED_GET_PARAM_REF(UInt16, netIFErrP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigMakeActive(void) {
        PRINTF("NetLibConfigMakeActive");

        //	Err NetLibConfigMakeActive( UInt16 libRefNum, UInt16 configIndex)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 configIndex");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigList(void) {
        PRINTF("NetLibConfigList");

        //	Err NetLibConfigList( UInt16 libRefNum, NetConfigNameType nameArray[],
        //						UInt16 *arrayEntriesP)

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, NetConfigNameType* nameArray,"
                     "UInt16 *arrayEntriesP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigIndexFromName(void) {
        PRINTF("NetLibConfigIndexFromName");

        //	Err NetLibConfigIndexFromName( UInt16 libRefNum, NetConfigNamePtr nameP,
        //						UInt16 *indexP)

        CALLED_SETUP("Err", "UInt16 libRefNum, NetConfigNamePtr nameP, UInt16 *indexP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigDelete(void) {
        PRINTF("NetLibConfigDelete");

        //	Err NetLibConfigDelete( UInt16 libRefNum, UInt16 index)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 index");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigSaveAs(void) {
        PRINTF("NetLibConfigSaveAs");

        //	Err NetLibConfigSaveAs( UInt16 libRefNum, NetConfigNamePtr nameP)

        CALLED_SETUP("Err", "UInt16 libRefNum, NetConfigNamePtr nameP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigRename(void) {
        PRINTF("NetLibConfigRename");

        //	Err NetLibConfigRename( UInt16 libRefNum, UInt16 index,
        //						NetConfigNamePtr newNameP)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 index, NetConfigNamePtr newNameP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigAliasSet(void) {
        PRINTF("NetLibConfigAliasSet");

        //	Err NetLibConfigAliasSet( UInt16 libRefNum, UInt16 configIndex,
        //						UInt16 aliasToIndex)

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 configIndex, UInt16 aliasToIndex");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigAliasGet(void) {
        PRINTF("NetLibConfigAliasGet");

        //	Err NetLibConfigAliasGet( UInt16 libRefNum, UInt16 aliasIndex,
        //						UInt16 *indexP, Boolean *isAnotherAliasP)

        CALLED_SETUP(
            "Err", "UInt16 libRefNum, UInt16 aliasIndex, UInt16 *indexP, Boolean *isAnotherAliasP");

        return kExecuteROM;
    }

    ProtoPatchTableEntry protoPatchTable[] = {
        {sysLibTrapOpen, HeadpatchNetLibOpen, NULL},
        {netLibOpenConfig, HeadpatchNetLibOpenConfig, NULL},
        {sysLibTrapClose, HeadpatchNetLibClose, NULL},
        {sysLibTrapSleep, HeadpatchNetLibSleep, NULL},
        {sysLibTrapWake, HeadpatchNetLibWake, NULL},
        {netLibTrapAddrINToA, HeadpatchNetLibAddrINToA, NULL},
        {netLibTrapAddrAToIN, HeadpatchNetLibAddrAToIN, NULL},
        {netLibTrapSocketOpen, HeadpatchNetLibSocketOpen, NULL},
        {netLibTrapSocketClose, HeadpatchNetLibSocketClose, NULL},
        {netLibTrapSocketOptionSet, HeadpatchNetLibSocketOptionSet, NULL},
        {netLibTrapSocketOptionGet, HeadpatchNetLibSocketOptionGet, NULL},
        {netLibTrapSocketBind, HeadpatchNetLibSocketBind, NULL},
        {netLibTrapSocketConnect, HeadpatchNetLibSocketConnect, NULL},
        {netLibTrapSocketListen, HeadpatchNetLibSocketListen, NULL},
        {netLibTrapSocketAccept, HeadpatchNetLibSocketAccept, NULL},
        {netLibTrapSocketShutdown, HeadpatchNetLibSocketShutdown, NULL},
        {netLibTrapSendPB, HeadpatchNetLibSendPB, NULL},
        {netLibTrapSend, HeadpatchNetLibSend, NULL},
        {netLibTrapReceivePB, HeadpatchNetLibReceivePB, NULL},
        {netLibTrapReceive, HeadpatchNetLibReceive, NULL},
        {netLibTrapDmReceive, HeadpatchNetLibDmReceive, NULL},
        {netLibTrapSelect, HeadpatchNetLibSelect, NULL},
        {netLibTrapMaster, HeadpatchNetLibMaster, NULL},
        {netLibTrapGetHostByName, HeadpatchNetLibGetHostByName, NULL},
        {netLibTrapSettingGet, HeadpatchNetLibSettingGet, NULL},
        {netLibTrapSettingSet, HeadpatchNetLibSettingSet, NULL},
        {netLibTrapIFAttach, HeadpatchNetLibIFAttach, NULL},
        {netLibTrapIFDetach, HeadpatchNetLibIFDetach, NULL},
        {netLibTrapIFGet, HeadpatchNetLibIFGet, NULL},
        {netLibTrapIFSettingGet, HeadpatchNetLibIFSettingGet, NULL},
        {netLibTrapIFSettingSet, HeadpatchNetLibIFSettingSet, NULL},
        {netLibTrapIFUp, HeadpatchNetLibIFUp, NULL},
        {netLibTrapIFDown, HeadpatchNetLibIFDown, NULL},
        {netLibTrapGetHostByAddr, HeadpatchNetLibGetHostByAddr, NULL},
        {netLibTrapGetServByName, HeadpatchNetLibGetServByName, NULL},
        {netLibTrapSocketAddr, HeadpatchNetLibSocketAddr, NULL},
        {netLibTrapFinishCloseWait, HeadpatchNetLibFinishCloseWait, NULL},
        {netLibTrapGetMailExchangeByName, HeadpatchNetLibGetMailExchangeByName, NULL},
        {netLibTrapOpenCount, HeadpatchNetLibOpenCount, NULL},
        {netLibTrapTracePrintF, HeadpatchNetLibTracePrintF, NULL},
        {netLibTrapTracePutS, HeadpatchNetLibTracePutS, NULL},
        {netLibTrapOpenIfCloseWait, HeadpatchNetLibOpenIfCloseWait, NULL},
        {netLibTrapHandlePowerOff, HeadpatchNetLibHandlePowerOff, NULL},
        {netLibTrapConnectionRefresh, HeadpatchNetLibConnectionRefresh, NULL},
        {netLibConfigMakeActive, HeadpatchNetLibConfigMakeActive, NULL},
        {netLibConfigList, HeadpatchNetLibConfigList, NULL},
        {netLibConfigIndexFromName, HeadpatchNetLibConfigIndexFromName, NULL},
        {netLibConfigDelete, HeadpatchNetLibConfigDelete, NULL},
        {netLibConfigSaveAs, HeadpatchNetLibConfigSaveAs, NULL},
        {netLibConfigRename, HeadpatchNetLibConfigRename, NULL},
        {netLibConfigAliasSet, HeadpatchNetLibConfigAliasSet, NULL},
        {netLibConfigAliasGet, HeadpatchNetLibConfigAliasGet, NULL},

        {0, NULL, NULL}};
}  // namespace

PatchModuleNetlib::PatchModuleNetlib() { LoadProtoPatchTable(protoPatchTable); }
