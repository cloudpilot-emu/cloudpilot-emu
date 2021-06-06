#include "PatchModuleNetlib.h"

#include "EmCommon.h"
#include "Feature.h"
#include "Logging.h"
#include "Marshal.h"
#include "NetworkProxy.h"

#define LOGGING 1

#ifdef LOGGING
    #define PRINTF logging::printf
#else
    #define PRINTF(...) ;
#endif

namespace {
    constexpr size_t PACKAGE_BUF_SIZE = 256;

    uint8 packageBuf[PACKAGE_BUF_SIZE];
    size_t packageBufLen = 0;
    bool packagePending = false;

    const char* DecodeIfSetting(uint16 setting) {
        switch (setting) {
            case netIFSettingResetAll:
                return "netIFSettingResetAll";
            case netIFSettingUp:
                return "netIFSettingUp";
            case netIFSettingName:
                return "netIFSettingName";
            case netIFSettingReqIPAddr:
                return "netIFSettingReqIPAddr";
            case netIFSettingSubnetMask:
                return "netIFSettingSubnetMask";
            case netIFSettingBroadcast:
                return "netIFSettingBroadcast";
            case netIFSettingUsername:
                return "netIFSettingUsername";
            case netIFSettingPassword:
                return "netIFSettingPassword";
            case netIFSettingDialbackUsername:
                return "netIFSettingDialbackUsername";
            case netIFSettingDialbackPassword:
                return "netIFSettingDialbackPassword";
            case netIFSettingAuthUsername:
                return "netIFSettingAuthUsername";
            case netIFSettingAuthPassword:
                return "netIFSettingAuthPassword";
            case netIFSettingServiceName:
                return "netIFSettingServiceName";
            case netIFSettingLoginScript:
                return "netIFSettingLoginScript";
            case netIFSettingConnectLog:
                return "netIFSettingConnectLog";
            case netIFSettingInactivityTimeout:
                return "netIFSettingInactivityTimeout";
            case netIFSettingEstablishmentTimeout:
                return "netIFSettingEstablishmentTimeout";
            case netIFSettingDynamicIP:
                return "netIFSettingDynamicIP";
            case netIFSettingVJCompEnable:
                return "netIFSettingVJCompEnable";
            case netIFSettingVJCompSlots:
                return "netIFSettingVJCompSlots";
            case netIFSettingMTU:
                return "netIFSettingMTU";
            case netIFSettingAsyncCtlMap:
                return "netIFSettingAsyncCtlMap";
            case netIFSettingPortNum:
                return "netIFSettingPortNum";
            case netIFSettingBaudRate:
                return "netIFSettingBaudRate";
            case netIFSettingFlowControl:
                return "netIFSettingFlowControl";
            case netIFSettingStopBits:
                return "netIFSettingStopBits";
            case netIFSettingParityOn:
                return "netIFSettingParityOn";
            case netIFSettingParityEven:
                return "netIFSettingParityEven";
            case netIFSettingUseModem:
                return "netIFSettingUseModem";
            case netIFSettingPulseDial:
                return "netIFSettingPulseDial";
            case netIFSettingModemInit:
                return "netIFSettingModemInit";
            case netIFSettingModemPhone:
                return "netIFSettingModemPhone";
            case netIFSettingRedialCount:
                return "netIFSettingRedialCount";
            case netIFSettingPowerUp:
                return "netIFSettingPowerUp";
            case netIFSettingWireless:
                return "netIFSettingWireless";
            case netIFSettingDNSQuery:
                return "netIFSettingDNSQuery";
            case netIFSettingQuitOnTxFail:
                return "netIFSettingQuitOnTxFail";
            case netIFSettingQueueSize:
                return "netIFSettingQueueSize";
            case netIFSettingTxInQueue:
                return "netIFSettingTxInQueue";
            case netIFSettingTxSent:
                return "netIFSettingTxSent";
            case netIFSettingTxDiscard:
                return "netIFSettingTxDiscard";
            case netIFSettingRssi:
                return "netIFSettingRssi";
            case netIFSettingRssiAsPercent:
                return "netIFSettingRssiAsPercent";
            case netIFSettingRadioState:
                return "netIFSettingRadioState";
            case netIFSettingBase:
                return "netIFSettingBase";
            case netIFSettingRadioID:
                return "netIFSettingRadioID";
            case netIFSettingBattery:
                return "netIFSettingBattery";
            case netIFSettingNetworkLoad:
                return "netIFSettingNetworkLoad";
            case netIFSettingConnectionName:
                return "netIFSettingConnectionName";
            case netIFSettingTraceBits:
                return "netIFSettingTraceBits";
            case netIFSettingGlobalsPtr:
                return "netIFSettingGlobalsPtr";
            case netIFSettingActualIPAddr:
                return "netIFSettingActualIPAddr";
            case netIFSettingServerIPAddr:
                return "netIFSettingServerIPAddr";
            case netIFSettingBringDownOnPowerDown:
                return "netIFSettingBringDownOnPowerDown";
            case netIFSettingRawMode:
                return "netIFSettingRawMode";
        }

        return "unknown";
    }

    const char* decodeCreator(uint32 creator) {
        static char buf[5];

        buf[0] = creator >> 24;
        buf[1] = (creator >> 16) & 0xff;
        buf[2] = (creator >> 8) & 0xff;
        buf[3] = creator & 0xff;
        buf[4] = 0;

        return buf;
    }

    void Hexdump(const uint8* buf, size_t bufSize) {
        printf("===BEGIN OF HEXDUMP===");

        for (size_t i = 0; i < bufSize; i++) {
            if (i % 16 == 0) putchar('\n');

            printf("%02x ", buf[i]);
        }

        printf("\n====END OF HEXDUMP====\n");
    }

    CallROMType HeadpatchNetLibOpen(void) {
        PRINTF("\nNetLibOpen");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 *netIFErrsP");

        if (Feature::GetNetworkRedirection()) {
            gNetworkProxy.Open();

            return kSkipROM;
        }

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_REF(UInt16, netIFErrsP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibClose(void) {
        PRINTF("\nNetLibClose");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 immediate");

        if (Feature::GetNetworkRedirection()) {
            PUT_RESULT_VAL(Err, gNetworkProxy.Close());

            return kSkipROM;
        }

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(UInt16, immediate);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSleep(void) {
        PRINTF("\nNetLibSleep");

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibWake(void) {
        PRINTF("\nNetLibWake");

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibAddrINToA(void) {
        PRINTF("\nNetLibAddrINToA");

        CALLED_SETUP("Char*", "UInt16 libRefNum, NetIPAddr inet, Char *spaceP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibAddrAToIN(void) {
        PRINTF("\nNetLibAddrAToIN");

        CALLED_SETUP("NetIPAddr", "UInt16 libRefNum, Char *a");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketOpen(void) {
        PRINTF("\nNetLibSocketOpen");

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

        if (Feature::GetNetworkRedirection()) {
            gNetworkProxy.SocketOpen(domain, type, protocol);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketClose(void) {
        PRINTF("\nNetLibSocketClose");

        CALLED_SETUP("Int16", "UInt16 libRefNum, NetSocketRef socket, Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketOptionSet(void) {
        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, NetSocketRef socket,"
                     "UInt16 level, UInt16 option, "
                     "void *optValueP, UInt16 optValueLen,"
                     "Int32 timeout, Err *errP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(NetSocketRef, socket);
        CALLED_GET_PARAM_VAL(UInt16, level);
        CALLED_GET_PARAM_VAL(UInt16, option);
        CALLED_GET_PARAM_VAL(UInt32, optValueP);
        CALLED_GET_PARAM_VAL(UInt16, optValueLen);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_REF(Err, errP, Marshal::kOutput);

        PRINTF("\nNetLibSocketOptionSet, option = 0x%04x", option);

        if (Feature::GetNetworkRedirection()) {
            gNetworkProxy.SocketOptionSet(socket, level, option, optValueP, optValueLen);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketOptionGet(void) {
        PRINTF("\nNetLibSocketOptionGet");

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
        PRINTF("\nNetLibSocketBind");

        if (Feature::GetNetworkRedirection()) {
            CALLED_SETUP("Int16",
                         "UInt16 libRefNum, NetSocketRef socket,"
                         "NetSocketAddrType *sockAddrP, Int16 addrLen, Int32 timeout, "
                         "Err *errP");

            CALLED_GET_PARAM_VAL(NetSocketRef, socket);
            CALLED_GET_PARAM_REF(NetSocketAddrType, sockAddrP, Marshal::kInput);

            gNetworkProxy.SocketBind(socket, sockAddrP);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSocketConnect(void) {
        PRINTF("\nNetLibSocketConnect");

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
        PRINTF("\nNetLibSocketListen");

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
        PRINTF("\nNetLibSocketAccept");

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
        PRINTF("\nNetLibSocketShutdown");

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
        PRINTF("\nNetLibSendPB");

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
        CALLED_GET_PARAM_PTR(uint8, bufP, bufLen, Marshal::kInput);

        PRINTF("\nNetLibSend, bufLen = %u", bufLen);
        Hexdump(bufP, bufLen);

        if (bufLen <= PACKAGE_BUF_SIZE) {
            memmove(packageBuf, bufP, bufLen);
            packageBufLen = bufLen;
            packagePending = true;

            *errP = 0;
            CALLED_PUT_PARAM_REF(errP);

            PUT_RESULT_VAL(Int16, bufLen);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibReceivePB(void) {
        PRINTF("\nNetLibReceivePB");

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
        CALLED_GET_PARAM_PTR(UInt8, bufP, bufLen, Marshal::kInOut);

        PRINTF("\nNetLibReceive, bufLen = %u", bufLen);

        if (packagePending && packageBufLen <= bufLen) {
            memmove(bufP, packageBuf, packageBufLen);
            memmove((uint8*)bufP + 12, packageBuf + 16, 4);
            memmove((uint8*)bufP + 16, packageBuf + 12, 4);

            ((uint8*)bufP)[20] = 0x00;

            uint16 checksum = packageBuf[22] | (packageBuf[23] << 8);
            checksum += 8;
            ((uint8*)bufP)[22] = checksum & 0xff;
            ((uint8*)bufP)[23] = (checksum >> 8) & 0xff;

            packagePending = false;

            CALLED_PUT_PARAM_REF(bufP);

            *errP = 0;
            CALLED_PUT_PARAM_REF(errP);

            PUT_RESULT_VAL(Int16, packageBufLen);

            Hexdump((uint8*)bufP, packageBufLen);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibDmReceive(void) {
        PRINTF("\nNetLibDmReceive");

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
        PRINTF("\nNetLibSelect");

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
        PRINTF("\nNetLibMaster");

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 cmd, NetMasterPBPtr pbP,"
                     "Int32 timeout");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibGetHostByName(void) {
        PRINTF("\nNetLibGetHostByName");

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
        PRINTF("\nNetLibSettingGet");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 setting, void *valueP, UInt16 *valueLenP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibSettingSet(void) {
        PRINTF("\nNetLibSettingSet");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 setting, void *valueP, UInt16 valueLen");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFAttach(void) {
        CALLED_SETUP("Err", "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance, Int32 timeout");

        CALLED_GET_PARAM_VAL(UInt16, ifInstance);
        CALLED_GET_PARAM_VAL(Int32, timeout);
        CALLED_GET_PARAM_VAL(UInt32, ifCreator);

        PRINTF("\nNetLibIFAttach, instance = %u, creator = %s, timeout = %i", ifInstance,
               decodeCreator(ifCreator), timeout);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFDetach(void) {
        PRINTF("\nNetLibIFDetach");

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,"
                     "Int32 timeout");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFGet(void) {
        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 index, UInt32 *ifCreatorP, "
                     "UInt16 *ifInstanceP");

        CALLED_GET_PARAM_VAL(UInt16, index);

        PRINTF("\nNetLibIFGet, index = %u", index);

        return kExecuteROM;
    }

    void TailpatchNetLibIFGet(void) {
        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 index, UInt32 *ifCreatorP, "
                     "UInt16 *ifInstanceP");

        CALLED_GET_PARAM_REF(UInt16, ifInstanceP, Marshal::kInput);
        CALLED_GET_PARAM_REF(UInt32, ifCreatorP, Marshal::kInput);
        GET_RESULT_VAL(Err);

        PRINTF("\n-> NetLibIFGet, result = %u, instance = %u, creator = %s", result, *ifInstanceP,
               decodeCreator(*ifCreatorP));
    }

    CallROMType HeadpatchNetLibIFSettingGet(void) {
        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,"
                     "UInt16 setting, void *valueP, UInt16 *valueLenP");

        CALLED_GET_PARAM_VAL(UInt16, ifInstance);
        CALLED_GET_PARAM_VAL(UInt16, setting);
        CALLED_GET_PARAM_VAL(UInt32, ifCreator);
        CALLED_GET_PARAM_REF(UInt16, valueLenP, Marshal::kInOut);
        CALLED_GET_PARAM_VAL(UInt32, valueP);

        PRINTF("\nNetLibIFSettingGet, instance = %u, creator = %s, setting = %s, valueLenP = %u",
               ifInstance, decodeCreator(ifCreator), DecodeIfSetting(setting), *valueLenP);

        if (setting == netIFSettingUp && *valueLenP > 0) {
            EmMemPut8(valueP, 1);
            *valueLenP = 1;

            CALLED_PUT_PARAM_REF(valueLenP);
            PUT_RESULT_VAL(Err, 0);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    void TailpatchNetLibIFSettingGet(void) {
        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,"
                     "UInt16 setting, void *valueP, UInt16 *valueLenP");

        CALLED_GET_PARAM_VAL(UInt16, setting);
        CALLED_GET_PARAM_VAL(UInt32, valueP);
        CALLED_GET_PARAM_REF(UInt16, valueLenP, Marshal::kInOut);

        if (setting == netIFSettingUp && *valueLenP > 0) {
            PRINTF("\n->NetLibIFSettingGet, netIFSettingUp = %u", (uint32)EmMemGet8(valueP));
        }
    }

    CallROMType HeadpatchNetLibIFSettingSet(void) {
        PRINTF("\nNetLibIFSettingSet");

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance,"
                     "UInt16 setting, void *valueP, UInt16 valueLen");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibIFUp(void) {
        CALLED_SETUP("Err", "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance");

        CALLED_GET_PARAM_VAL(UInt16, ifInstance);
        CALLED_GET_PARAM_VAL(UInt32, ifCreator);

        PRINTF("\nNetLibIFUp, instance = %u, creator = %s", ifInstance, decodeCreator(ifCreator));

        PUT_RESULT_VAL(Err, 0);

        return kSkipROM;
    }

    void TailpatchNetLibIFUp(void) {
        CALLED_SETUP("Err", "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance");

        GET_RESULT_VAL(Err);

        PRINTF("\n-> NetLibIFUp, res = %u", result & 0xff);
    }

    CallROMType HeadpatchNetLibIFDown(void) {
        PRINTF("\nNetLibIFDown");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt32 ifCreator, UInt16 ifInstance, Int32 timeout");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibGetHostByAddr(void) {
        PRINTF("\nNetLibGetHostByAddr");

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
        PRINTF("\nNetLibGetServByName");

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
        PRINTF("\nNetLibSocketAddr");

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

        if (Feature::GetNetworkRedirection()) {
            gNetworkProxy.SocketAddr(socket);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibFinishCloseWait(void) {
        PRINTF("\nNetLibFinishCloseWait");

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibGetMailExchangeByName(void) {
        PRINTF("\nNetLibGetMailExchangeByName");

        CALLED_SETUP("Int16",
                     "UInt16 libRefNum, Char *mailNameP, "
                     "UInt16 maxEntries, "
                     "Char** hostNames, UInt16* priorities, "
                     "Int32	timeout, Err *errP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibOpenCount(void) {
        PRINTF("\nNetLibOpenCount");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 *countP");

        if (Feature::GetNetworkRedirection()) {
            CALLED_GET_PARAM_REF(UInt16, countP, Marshal::kOutput);

            *countP = gNetworkProxy.OpenCount();

            CALLED_PUT_PARAM_REF(countP);

            cout << "put: " << *countP << endl << flush;

            PUT_RESULT_VAL(Err, 0);

            return kSkipROM;
        }

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_REF(UInt16, countP, Marshal::kOutput);
        return kExecuteROM;
    }

    void TailpatchNetLibOpenCount() {
        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 *countP");

        CALLED_GET_PARAM_REF(UInt16, countP, Marshal::kInput);

        PRINTF("-> NetLibOpenCount, countP = %d", *countP);
    }

    CallROMType HeadpatchNetLibTracePrintF(void) {
        PRINTF("\nNetLibTracePrintF");

        CALLED_SETUP("Err", "UInt16 libRefNum, Char *formatStr");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibTracePutS(void) {
        PRINTF("\nNetLibTracePutS");

        CALLED_SETUP("Err", "UInt16 libRefNum, Char *strP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibOpenIfCloseWait(void) {
        PRINTF("\nNetLibOpenIfCloseWait");

        CALLED_SETUP("Err", "UInt16 libRefNum");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);

        if (Feature::GetNetworkRedirection()) {
            PUT_RESULT_VAL(Err, netErrNotOpen);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibHandlePowerOff(void) {
        PRINTF("\nNetLibHandlePowerOff");

        CALLED_SETUP("Err", "UInt16 libRefNum, SysEventType *eventP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_REF(EventType, eventP, Marshal::kInput);
        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConnectionRefresh(void) {
        CALLED_SETUP("Err",
                     "UInt16 libRefNum, Boolean refresh, "
                     "UInt8 *allInterfacesUpP, UInt16 *netIFErrP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(Boolean, refresh);
        CALLED_GET_PARAM_REF(Boolean, allInterfacesUpP, Marshal::kOutput);
        CALLED_GET_PARAM_REF(UInt16, netIFErrP, Marshal::kOutput);

        PRINTF("\nNetLibConnectionRefresh, refresh = %u", (uint16)refresh);

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibOpenConfig(void) {
        CALLED_SETUP("Err",
                     "UInt16 libRefNum, UInt16 configIndex, UInt32 openFlags,"
                     "UInt16 *netIFErrP");

        CALLED_GET_PARAM_VAL(UInt16, libRefNum);
        CALLED_GET_PARAM_VAL(UInt16, configIndex);
        CALLED_GET_PARAM_VAL(UInt32, openFlags);
        CALLED_GET_PARAM_REF(UInt16, netIFErrP, Marshal::kOutput);

        PRINTF("\nNetLibOpenConfig, configIndex = %u", configIndex);

        if (Feature::GetNetworkRedirection()) {
            gNetworkProxy.Open();

            *netIFErrP = 0;
            CALLED_PUT_PARAM_REF(netIFErrP);

            return kSkipROM;
        }

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigMakeActive(void) {
        PRINTF("\nNetLibConfigMakeActive");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 configIndex");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigList(void) {
        PRINTF("\nNetLibConfigList");

        CALLED_SETUP("Err",
                     "UInt16 libRefNum, NetConfigNameType* nameArray,"
                     "UInt16 *arrayEntriesP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigIndexFromName(void) {
        PRINTF("\nNetLibConfigIndexFromName");

        CALLED_SETUP("Err", "UInt16 libRefNum, NetConfigNamePtr nameP, UInt16 *indexP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigDelete(void) {
        PRINTF("\nNetLibConfigDelete");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 index");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigSaveAs(void) {
        PRINTF("\nNetLibConfigSaveAs");

        CALLED_SETUP("Err", "UInt16 libRefNum, NetConfigNamePtr nameP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigRename(void) {
        PRINTF("\nNetLibConfigRename");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 index, NetConfigNamePtr newNameP");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigAliasSet(void) {
        PRINTF("\nNetLibConfigAliasSet");

        CALLED_SETUP("Err", "UInt16 libRefNum, UInt16 configIndex, UInt16 aliasToIndex");

        return kExecuteROM;
    }

    CallROMType HeadpatchNetLibConfigAliasGet(void) {
        PRINTF("\nNetLibConfigAliasGet");

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
        {netLibTrapIFGet, HeadpatchNetLibIFGet, TailpatchNetLibIFGet},
        {netLibTrapIFSettingGet, HeadpatchNetLibIFSettingGet, TailpatchNetLibIFSettingGet},
        {netLibTrapIFSettingSet, HeadpatchNetLibIFSettingSet, NULL},
        {netLibTrapIFUp, HeadpatchNetLibIFUp, TailpatchNetLibIFUp},
        {netLibTrapIFDown, HeadpatchNetLibIFDown, NULL},
        {netLibTrapGetHostByAddr, HeadpatchNetLibGetHostByAddr, NULL},
        {netLibTrapGetServByName, HeadpatchNetLibGetServByName, NULL},
        {netLibTrapSocketAddr, HeadpatchNetLibSocketAddr, NULL},
        {netLibTrapFinishCloseWait, HeadpatchNetLibFinishCloseWait, NULL},
        {netLibTrapGetMailExchangeByName, HeadpatchNetLibGetMailExchangeByName, NULL},
        {netLibTrapOpenCount, HeadpatchNetLibOpenCount, TailpatchNetLibOpenCount},
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
