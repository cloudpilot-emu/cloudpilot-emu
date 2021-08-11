code = `
case netSocketOptLevelIP:
			outOptLevel = IPPROTO_IP;
			switch (inOptName)
			{
#ifdef IP_OPTIONS
				case netSocketOptIPOptions:				// Not supported by WinSock 1.x
					outOptName = IP_OPTIONS;
					goto Common;
#endif
				default:	// Tell gcc to shut up about unhandled enumeration values.
					break;
			}
			break;

		case netSocketOptLevelTCP:
			outOptLevel = IPPROTO_TCP;
			switch (inOptName)
			{
#ifdef TCP_NODELAY
				case netSocketOptTCPNoDelay:
					outOptName = TCP_NODELAY;
					goto Common;
#endif

#ifdef TCP_MAXSEG
				case netSocketOptTCPMaxSeg:				// Not supported by WinSock
					outOptName = TCP_MAXSEG;
					goto Common;
#endif
				default:	// Tell gcc to shut up about unhandled enumeration values.
					break;
			}
			break;

		case netSocketOptLevelSocket:
			outOptLevel = SOL_SOCKET;
			switch (inOptName)
			{
#ifdef SO_DEBUG
				case netSocketOptSockDebug:
					outOptName = SO_DEBUG;
					goto Common;
#endif

#ifdef SO_ACCEPTCONN
				case netSocketOptSockAcceptConn:
					outOptName = SO_ACCEPTCONN;
					goto Common;
#endif

#ifdef SO_REUSEADDR
				case netSocketOptSockReuseAddr:
					outOptName = SO_REUSEADDR;
					goto Common;
#endif

#ifdef SO_KEEPALIVE
				case netSocketOptSockKeepAlive:
					outOptName = SO_KEEPALIVE;
					goto Common;
#endif

#ifdef SO_DONTROUTE
				case netSocketOptSockDontRoute:
					outOptName = SO_DONTROUTE;
					goto Common;
#endif

#ifdef SO_BROADCAST
				case netSocketOptSockBroadcast:
					outOptName = SO_BROADCAST;
					goto Common;
#endif

#ifdef SO_USELOOPBACK
				case netSocketOptSockUseLoopback:
					outOptName = SO_USELOOPBACK;
					goto Common;
#endif

#ifdef SO_LINGER
				case netSocketOptSockLinger:
					outOptName = SO_LINGER;
					if (inOptValLen == sizeof (NetSocketLingerType))
					{
						outOptValLen = sizeof (linger);
						outOptVal = (char*) Platform::AllocateMemory (outOptValLen);

						NetSocketLingerType	val = *(NetSocketLingerType*) inOptVal;

						Canonical (val.onOff);	// Byteswap on LE machines
						Canonical (val.time);	// Byteswap on LE machines

						((linger*) outOptVal)->l_onoff = val.onOff;
						((linger*) outOptVal)->l_linger = val.time;

						result = true;
					}
					break;
#endif

#ifdef SO_OOBINLINE
				case netSocketOptSockOOBInLine:
					outOptName = SO_OOBINLINE;
					goto Common;
#endif

#ifdef SO_SNDBUF
				case netSocketOptSockSndBufSize:
					outOptName = SO_SNDBUF;
					goto Common;
#endif

#ifdef SO_RCVBUF
				case netSocketOptSockRcvBufSize:
					outOptName = SO_RCVBUF;
					goto Common;
#endif

#ifdef SO_SNDLOWAT
				case netSocketOptSockSndLowWater:		// Not supported by WinSock
					outOptName = SO_SNDLOWAT;
					goto Common;
#endif

#ifdef SO_RCVLOWAT
				case netSocketOptSockRcvLowWater:		// Not supported by WinSock
					outOptName = SO_RCVLOWAT;
					goto Common;
#endif

#ifdef SO_SNDTIMEO
				case netSocketOptSockSndTimeout:		// Not supported by WinSock
					outOptName = SO_SNDTIMEO;
					goto Common;
#endif

#ifdef SO_RCVTIMEO
				case netSocketOptSockRcvTimeout:		// Not supported by WinSock
					outOptName = SO_RCVTIMEO;
					goto Common;
#endif

#ifdef SO_ERROR
				case netSocketOptSockErrorStatus:
					outOptName = SO_ERROR;
					goto Common;
#endif

#ifdef SO_TYPE
				case netSocketOptSockSocketType:
					outOptName = SO_TYPE;
					goto Common;
`;

let key;

for (let line of code.split('\n')) {
    let match;

    if ((match = line.match(/^\s*case\s+(\w+)\s*:/))) key = match[1];
    if ((match = line.match(/^\s*outOptName\s*=\s*(\w+)\s*;/))) {
        console.log(`    ${key}: ${match[1]}`);
        key = undefined;
    }
}
