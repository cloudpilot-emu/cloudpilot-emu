import errno
import socket

netErrorClass = 0x1200

netErrAlreadyOpen = netErrorClass | 1
netErrNotOpen = netErrorClass | 2
netErrStillOpen = netErrorClass | 3
netErrParamErr = netErrorClass | 4
netErrNoMoreSockets = netErrorClass | 5
netErrOutOfResources = netErrorClass | 6
netErrOutOfMemory = netErrorClass | 7
netErrSocketNotOpen = netErrorClass | 8
netErrSocketBusy = netErrorClass | 9
netErrMessageTooBig = netErrorClass | 10
netErrSocketNotConnected = netErrorClass | 11
netErrNoInterfaces = netErrorClass | 12
netErrBufTooSmall = netErrorClass | 13
netErrUnimplemented = netErrorClass | 14
netErrPortInUse = netErrorClass | 15
netErrQuietTimeNotElapsed = netErrorClass | 16
netErrInternal = netErrorClass | 17
netErrTimeout = netErrorClass | 18
netErrSocketAlreadyConnected = netErrorClass | 19
netErrSocketClosedByRemote = netErrorClass | 20
netErrOutOfCmdBlocks = netErrorClass | 21
netErrWrongSocketType = netErrorClass | 22
netErrSocketNotListening = netErrorClass | 23
netErrUnknownSetting = netErrorClass | 24
netErrInvalidSettingSize = netErrorClass | 25
netErrPrefNotFound = netErrorClass | 26
netErrInvalidInterface = netErrorClass | 27
netErrInterfaceNotFound = netErrorClass | 28
netErrTooManyInterfaces = netErrorClass | 29
netErrBufWrongSize = netErrorClass | 30
netErrUserCancel = netErrorClass | 31
netErrBadScript = netErrorClass | 32
netErrNoSocket = netErrorClass | 33
netErrSocketRcvBufFull = netErrorClass | 34
netErrNoPendingConnect = netErrorClass | 35
netErrUnexpectedCmd = netErrorClass | 36
netErrNoTCB = netErrorClass | 37
netErrNilRemoteWindowSize = netErrorClass | 38
netErrNoTimerProc = netErrorClass | 39
netErrSocketInputShutdown = netErrorClass | 40
netErrCmdBlockNotCheckedOut = netErrorClass | 41
netErrCmdNotDone = netErrorClass | 42
netErrUnknownProtocol = netErrorClass | 43
netErrUnknownService = netErrorClass | 44
netErrUnreachableDest = netErrorClass | 45
netErrReadOnlySetting = netErrorClass | 46
netErrWouldBlock = netErrorClass | 47
netErrAlreadyInProgress = netErrorClass | 48
netErrPPPTimeout = netErrorClass | 49
netErrPPPBroughtDown = netErrorClass | 50
netErrAuthFailure = netErrorClass | 51
netErrPPPAddressRefused = netErrorClass | 52
netErrDNSNameTooLong = netErrorClass | 53
netErrDNSBadName = netErrorClass | 54
netErrDNSBadArgs = netErrorClass | 55
netErrDNSLabelTooLong = netErrorClass | 56
netErrDNSAllocationFailure = netErrorClass | 57
netErrDNSTimeout = netErrorClass | 58
netErrDNSUnreachable = netErrorClass | 59
netErrDNSFormat = netErrorClass | 60
netErrDNSServerFailure = netErrorClass | 61
netErrDNSNonexistantName = netErrorClass | 62
netErrDNSNIY = netErrorClass | 63
netErrDNSRefused = netErrorClass | 64
netErrDNSImpossible = netErrorClass | 65
netErrDNSNoRRS = netErrorClass | 66
netErrDNSAborted = netErrorClass | 67
netErrDNSBadProtocol = netErrorClass | 68
netErrDNSTruncated = netErrorClass | 69
netErrDNSNoRecursion = netErrorClass | 70
netErrDNSIrrelevant = netErrorClass | 71
netErrDNSNotInLocalCache = netErrorClass | 72
netErrDNSNoPort = netErrorClass | 73
netErrIPCantFragment = netErrorClass | 74
netErrIPNoRoute = netErrorClass | 75
netErrIPNoSrc = netErrorClass | 76
netErrIPNoDst = netErrorClass | 77
netErrIPktOverflow = netErrorClass | 78
netErrTooManyTCPConnections = netErrorClass | 79
netErrNoDNSServers = netErrorClass | 80
netErrInterfaceDown = netErrorClass | 81
netErrNoChannel = netErrorClass | 82
netErrDieState = netErrorClass | 83
netErrReturnedInMail = netErrorClass | 84
netErrReturnedNoTransfer = netErrorClass | 85
netErrReturnedIllegal = netErrorClass | 86
netErrReturnedCongest = netErrorClass | 87
netErrReturnedError = netErrorClass | 88
netErrReturnedBusy = netErrorClass | 89
netErrGMANState = netErrorClass | 90
netErrQuitOnTxFail = netErrorClass | 91
netErrFlexListFull = netErrorClass | 92
netErrSenderMAN = netErrorClass | 93
netErrIllegalType = netErrorClass | 94
netErrIllegalState = netErrorClass | 95
netErrIllegalFlags = netErrorClass | 96
netErrIllegalSendlist = netErrorClass | 97
netErrIllegalMPAKLength = netErrorClass | 98
netErrIllegalAddressee = netErrorClass | 99
netErrIllegalPacketClass = netErrorClass | 100
netErrBufferLength = netErrorClass | 101
netErrNiCdLowBattery = netErrorClass | 102
netErrRFinterfaceFatal = netErrorClass | 103
netErrIllegalLogout = netErrorClass | 104
netErrAAARadioLoad = netErrorClass | 105
netErrAntennaDown = netErrorClass | 106
netErrNiCdCharging = netErrorClass | 107
netErrAntennaWentDown = netErrorClass | 108
netErrNotActivated = netErrorClass | 109
netErrRadioTemp = netErrorClass | 110
netErrNiCdChargeError = netErrorClass | 111
netErrNiCdSag = netErrorClass | 112
netErrNiCdChargeSuspend = netErrorClass | 113
netErrConfigNotFound = netErrorClass | 115
netErrConfigCantDelete = netErrorClass | 116
netErrConfigTooMany = netErrorClass | 117
netErrConfigBadName = netErrorClass | 118
netErrConfigNotAlias = netErrorClass | 119
netErrConfigCantPointToAlias = netErrorClass | 120
netErrConfigEmpty = netErrorClass | 121
netErrAlreadyOpenWithOtherConfig = netErrorClass | 122
netErrConfigAliasErr = netErrorClass | 123
netErrNoMultiPktAddr = netErrorClass | 124
netErrOutOfPackets = netErrorClass | 125
netErrMultiPktAddrReset = netErrorClass | 126
netErrStaleMultiPktAddr = netErrorClass | 127
netErrScptPluginMissing = netErrorClass | 128
netErrScptPluginLaunchFail = netErrorClass | 129
netErrScptPluginCmdFail = netErrorClass | 130
netErrScptPluginInvalidCmd = netErrorClass | 131

_errnoNameLookup = {
    'EINTR': netErrUserCancel,
    'EDEADLK': netErrWouldBlock,
    'ENOMEM': netErrOutOfMemory,
    'EACCES': netErrAuthFailure,
    'EBUSY': netErrSocketBusy,
    'EROFS': netErrReadOnlySetting,
    'EAGAIN': netErrWouldBlock,
    'EWOULDBLOCK': netErrWouldBlock,
    'EINPROGRESS': netErrWouldBlock,
    'EALREADY': netErrAlreadyInProgress,
    'ENOTSOCK': netErrNoSocket,
    'EDESTADDRREQ': netErrIPNoDst,
    'EMSGSIZE': netErrMessageTooBig,
    'ENOPROTOOPT': netErrUnknownProtocol,
    'EPROTONOSUPPORT': netErrUnknownProtocol,
    'ESOCKTNOSUPPORT': netErrWrongSocketType,
    'EOPNOTSUPP': netErrWrongSocketType,
    'EPFNOSUPPORT': netErrUnknownService,
    'EAFNOSUPPORT': netErrUnknownService,
    'EADDRINUSE': netErrPortInUse,
    'EADDRNOTAVAIL': netErrPortInUse,
    'ENETDOWN': netErrUnreachableDest,
    'ENETUNREACH': netErrNoInterfaces,
    'ENETRESET': netErrSocketClosedByRemote,
    'ECONNABORTED': netErrSocketClosedByRemote,
    'ECONNRESET': netErrSocketClosedByRemote,
    'ENOBUFS': netErrNoTCB,
    'EISCONN': netErrSocketAlreadyConnected,
    'ENOTCONN': netErrSocketNotConnected,
    'ESHUTDOWN': netErrSocketNotOpen,
    'ETIMEDOUT': netErrTimeout,
    'ECONNREFUSED': netErrTimeout,
    'EHOSTDOWN': netErrIPNoRoute,
    'EHOSTUNREACH': netErrIPNoRoute,
    'WSAEACCES': netErrWrongSocketType,
    'WSAEADDRINUSE': netErrPortInUse,
    'WSAEADDRNOTAVAIL': netErrParamErr,
    'WSAEAFNOSUPPORT': netErrParamErr,
    'WSAEALREADY': netErrAlreadyInProgress,
    'WSAECONNABORTED': netErrSocketClosedByRemote,
    'WSAECONNREFUSED': netErrTimeout,
    'WSAECONNRESET': netErrSocketClosedByRemote,
    'WSAEDESTADDRREQ': netErrParamErr,
    'WSAEFAULT': netErrParamErr,
    'WSAEHOSTDOWN': netErrTimeout,
    'WSAEHOSTUNREACH': netErrUnreachableDest,
    'WSAEINPROGRESS': netErrAlreadyInProgress,
    'WSAEINTR': netErrUserCancel,
    'WSAEINVAL': netErrParamErr,
    'WSAEISCONN': netErrSocketAlreadyConnected,
    'WSAEMFILE': netErrNoMoreSockets,
    'WSAEMSGSIZE': netErrMessageTooBig,
    'WSAENETDOWN': netErrUnreachableDest,
    'WSAENETRESET': netErrSocketClosedByRemote,
    'WSAENETUNREACH': netErrUnreachableDest,
    'WSAENOBUFS': netErrOutOfMemory,
    'WSAENOPROTOOPT': netErrParamErr,
    'WSAENOTCONN': netErrSocketNotConnected,
    'WSAENOTSOCK': netErrParamErr,
    'WSAEOPNOTSUPP': netErrWrongSocketType,
    'WSAEPFNOSUPPORT': netErrParamErr,
    'WSAEPROCLIM': netErrOutOfResources,
    'WSAEPROTONOSUPPORT': netErrUnknownProtocol,
    'WSAEPROTOTYPE': netErrUnknownProtocol,
    'WSAESHUTDOWN': netErrNotOpen,
    'WSAESOCKTNOSUPPORT': netErrParamErr,
    'WSAETIMEDOUT': netErrTimeout,
    'WSAEWOULDBLOCK': netErrWouldBlock,
    'WSAHOST_NOT_FOUND': netErrDNSNonexistantName,
    'WSANOTINITIALISED': netErrNotOpen,
    'WSANO_DATA': netErrDNSFormat,
    'WSANO_RECOVERY': netErrInternal,
    'WSASYSNOTREADY': netErrInternal,
    'WSATRY_AGAIN': netErrDNSTimeout,
    'WSAVERNOTSUPPORTED': netErrInternal,
    'WSAEDISCON': netErrSocketNotConnected
}

_errnoLookup = {}

for code, name in errno.errorcode.items():
    _errnoLookup[code] = _errnoNameLookup[name] if name in _errnoNameLookup else netErrInternal

_gaierrnoLookup = {
    socket.EAI_NONAME: netErrDNSUnreachable,
    socket.EAI_AGAIN: netErrDNSServerFailure,
    socket.EAI_FAIL: netErrDNSRefused,
    socket.EAI_NODATA: netErrDNSNonexistantName
}

_herrnoLookup = {
    1: netErrDNSUnreachable,
    2: netErrDNSServerFailure,
    3: netErrDNSRefused,
    4: netErrDNSNonexistantName,
    11001: netErrDNSUnreachable,
    11002: netErrDNSServerFailure,
    11003: netErrDNSRefused,
    11004: netErrDNSNonexistantName,
}


def errnoToPalm(errno: int) -> int:
    return _errnoLookup[errno] if errno in _errnoLookup else netErrInternal


def gaierrnoToPalm(gaierrno: int) -> int:
    return _gaierrnoLookup[gaierrno] if gaierrno in _gaierrnoLookup else netErrInternal


def herrnoToPalm(herrno: int) -> int:
    return _herrnoLookup[herrno] if herrno in _herrnoLookup else netErrInternal
