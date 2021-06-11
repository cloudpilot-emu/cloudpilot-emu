const codes = `
#define	netErrAlreadyOpen					(netErrorClass | 1)
#define	netErrNotOpen						(netErrorClass | 2)
#define	netErrStillOpen					(netErrorClass | 3)
#define	netErrParamErr						(netErrorClass | 4)
#define	netErrNoMoreSockets				(netErrorClass | 5)
#define	netErrOutOfResources				(netErrorClass | 6)
#define	netErrOutOfMemory					(netErrorClass | 7)
#define	netErrSocketNotOpen				(netErrorClass | 8)
#define	netErrSocketBusy					(netErrorClass | 9)		//EINPROGRESS
#define	netErrMessageTooBig				(netErrorClass | 10)
#define	netErrSocketNotConnected 		(netErrorClass | 11)
#define	netErrNoInterfaces				(netErrorClass | 12)		//ENETUNREACH
#define	netErrBufTooSmall					(netErrorClass | 13)
#define	netErrUnimplemented				(netErrorClass | 14)
#define	netErrPortInUse					(netErrorClass | 15)		//EADDRINUSE
#define	netErrQuietTimeNotElapsed		(netErrorClass | 16)		//EADDRINUSE
#define	netErrInternal						(netErrorClass | 17)
#define	netErrTimeout						(netErrorClass | 18)		//ETIMEDOUT
#define	netErrSocketAlreadyConnected 	(netErrorClass | 19)		//EISCONN
#define	netErrSocketClosedByRemote 	(netErrorClass | 20)
#define	netErrOutOfCmdBlocks			 	(netErrorClass | 21)
#define	netErrWrongSocketType			(netErrorClass | 22)
#define	netErrSocketNotListening		(netErrorClass | 23)
#define	netErrUnknownSetting				(netErrorClass | 24)
#define	netErrInvalidSettingSize		(netErrorClass | 25)
#define	netErrPrefNotFound				(netErrorClass | 26)
#define	netErrInvalidInterface			(netErrorClass | 27)
#define	netErrInterfaceNotFound			(netErrorClass | 28)
#define	netErrTooManyInterfaces			(netErrorClass | 29)
#define	netErrBufWrongSize				(netErrorClass | 30)
#define	netErrUserCancel					(netErrorClass | 31)
#define	netErrBadScript					(netErrorClass | 32)
#define	netErrNoSocket						(netErrorClass | 33)
#define	netErrSocketRcvBufFull			(netErrorClass | 34)
#define	netErrNoPendingConnect			(netErrorClass | 35)
#define	netErrUnexpectedCmd				(netErrorClass | 36)
#define	netErrNoTCB							(netErrorClass | 37)
#define	netErrNilRemoteWindowSize		(netErrorClass | 38)
#define	netErrNoTimerProc					(netErrorClass | 39)
#define	netErrSocketInputShutdown		(netErrorClass | 40)		// EOF to sockets API
#define	netErrCmdBlockNotCheckedOut	(netErrorClass | 41)
#define	netErrCmdNotDone					(netErrorClass | 42)
#define	netErrUnknownProtocol			(netErrorClass | 43)
#define	netErrUnknownService				(netErrorClass | 44)
#define	netErrUnreachableDest			(netErrorClass | 45)
#define	netErrReadOnlySetting			(netErrorClass | 46)
#define	netErrWouldBlock					(netErrorClass | 47)		//EWOULDBLOCK
#define	netErrAlreadyInProgress			(netErrorClass | 48)		//EALREADY
#define	netErrPPPTimeout					(netErrorClass | 49)
#define	netErrPPPBroughtDown				(netErrorClass | 50)
#define	netErrAuthFailure					(netErrorClass | 51)
#define	netErrPPPAddressRefused			(netErrorClass | 52)
// The following map into the Epilogue DNS errors declared in DNS.ep.h:
//  and MUST be kept in this order!!
#define	netErrDNSNameTooLong				(netErrorClass | 53)
#define	netErrDNSBadName					(netErrorClass | 54)
#define	netErrDNSBadArgs					(netErrorClass | 55)
#define	netErrDNSLabelTooLong			(netErrorClass | 56)
#define	netErrDNSAllocationFailure		(netErrorClass | 57)
#define	netErrDNSTimeout					(netErrorClass | 58)
#define	netErrDNSUnreachable				(netErrorClass | 59)
#define	netErrDNSFormat					(netErrorClass | 60)
#define	netErrDNSServerFailure			(netErrorClass | 61)
#define	netErrDNSNonexistantName		(netErrorClass | 62)
#define	netErrDNSNIY						(netErrorClass | 63)
#define	netErrDNSRefused					(netErrorClass | 64)
#define	netErrDNSImpossible				(netErrorClass | 65)
#define	netErrDNSNoRRS						(netErrorClass | 66)
#define	netErrDNSAborted					(netErrorClass | 67)
#define	netErrDNSBadProtocol				(netErrorClass | 68)
#define	netErrDNSTruncated				(netErrorClass | 69)
#define	netErrDNSNoRecursion				(netErrorClass | 70)
#define	netErrDNSIrrelevant				(netErrorClass | 71)
#define	netErrDNSNotInLocalCache		(netErrorClass | 72)
#define	netErrDNSNoPort					(netErrorClass | 73)
// The following map into the Epilogue IP errors declared in IP.ep.h:
//  and MUST be kept in this order!!
#define	netErrIPCantFragment				(netErrorClass | 74)
#define	netErrIPNoRoute					(netErrorClass | 75)
#define	netErrIPNoSrc						(netErrorClass | 76)
#define	netErrIPNoDst						(netErrorClass | 77)
#define	netErrIPktOverflow				(netErrorClass | 78)
// End of Epilogue IP errors
#define	netErrTooManyTCPConnections	(netErrorClass | 79)
#define  netErrNoDNSServers				(netErrorClass | 80)
#define	netErrInterfaceDown				(netErrorClass | 81)

// Mobitex network radio interface error code returns
#define	netErrNoChannel					(netErrorClass | 82)	// The datalink layer cannot acquire a channel
#define	netErrDieState						(netErrorClass | 83)	// Mobitex network has issued a DIE command.
#define	netErrReturnedInMail				(netErrorClass | 84) // The addressed of the transmitted packet was not available, and the message was placed in the network's mailbox.
#define	netErrReturnedNoTransfer		(netErrorClass | 85)	// This message cannot be transferred or put in the network mailbox.
#define	netErrReturnedIllegal			(netErrorClass | 86)	// The message could not be switched to the network
#define	netErrReturnedCongest			(netErrorClass | 87)	// Line, radio channels, or network nodes are congested.
#define	netErrReturnedError				(netErrorClass | 88)	// Technical error in the network.
#define	netErrReturnedBusy				(netErrorClass | 89)	// The B-party is busy.
#define	netErrGMANState					(netErrorClass | 90)	// The modem has not registered with the network.
#define	netErrQuitOnTxFail				(netErrorClass | 91) // Couldn't get packet through, shutdown.
#define	netErrFlexListFull				(netErrorClass | 92) // raw IF error message: see Mobitex spec.
#define	netErrSenderMAN					(netErrorClass | 93) // ditto
#define	netErrIllegalType					(netErrorClass | 94) // ditto
#define	netErrIllegalState				(netErrorClass | 95) // ditto
#define	netErrIllegalFlags				(netErrorClass | 96) // ditto
#define	netErrIllegalSendlist			(netErrorClass | 97)	// ditto
#define	netErrIllegalMPAKLength			(netErrorClass | 98)	// ditto
#define	netErrIllegalAddressee			(netErrorClass | 99)	// ditto
#define	netErrIllegalPacketClass		(netErrorClass | 100) // ditto
#define	netErrBufferLength				(netErrorClass | 101) // any
#define	netErrNiCdLowBattery				(netErrorClass | 102) // any
#define	netErrRFinterfaceFatal			(netErrorClass | 103) // any
#define	netErrIllegalLogout				(netErrorClass | 104) // raw IF error message
#define	netErrAAARadioLoad				(netErrorClass | 105)	// 7/20/98 JB.  If there is insufficient AAA
#define	netErrAntennaDown					(netErrorClass | 106)
#define	netErrNiCdCharging				(netErrorClass | 107)	// just for charging
#define	netErrAntennaWentDown			(netErrorClass | 108)
#define	netErrNotActivated				(netErrorClass | 109)	// The unit has not been FULLY activated.  George and Morty completed.
#define	netErrRadioTemp					(netErrorClass | 110)	// Radio's temp is too high for FCC compliant TX
#define	netErrNiCdChargeError			(netErrorClass | 111)	// Charging stopped due to NiCd charging characteristic
#define	netErrNiCdSag						(netErrorClass | 112)	// the computed sag or actual sag indicates a NiCd with diminished capacity.
#define	netErrNiCdChargeSuspend			(netErrorClass | 113)	// Charging has been suspended due to low AAA batteries.
// Left room for more Mobitex errors

// Configuration errors
#define	netErrConfigNotFound				(netErrorClass | 115)
#define	netErrConfigCantDelete			(netErrorClass | 116)
#define	netErrConfigTooMany				(netErrorClass | 117)
#define	netErrConfigBadName				(netErrorClass | 118)
#define	netErrConfigNotAlias				(netErrorClass | 119)
#define	netErrConfigCantPointToAlias	(netErrorClass | 120)
#define	netErrConfigEmpty					(netErrorClass | 121)
#define	netErrAlreadyOpenWithOtherConfig		(netErrorClass | 122)
#define	netErrConfigAliasErr				(netErrorClass | 123)
#define	netErrNoMultiPktAddr				(netErrorClass | 124)
#define	netErrOutOfPackets				(netErrorClass | 125)
#define	netErrMultiPktAddrReset			(netErrorClass | 126)
#define	netErrStaleMultiPktAddr			(netErrorClass | 127)

// Login scripting plugin errors
#define	netErrScptPluginMissing			(netErrorClass | 128)
#define	netErrScptPluginLaunchFail		(netErrorClass | 129)
#define	netErrScptPluginCmdFail			(netErrorClass | 130)
#define	netErrScptPluginInvalidCmd		(netErrorClass | 131)
`;

for (const line of codes.split('\n')) {
    const match = line.match(/^\s*#define\s+(\w+)\s+\(\s*netErrorClass\s+\|\s+(\d+)\s*\)/);

    if (!match) continue;

    const ident = match[1];

    console.log(`${ident} = netErrorClass | ${match[2]}`);
}
