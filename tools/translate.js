const code = `
//		case EPERM:				//	1		/* Operation not permitted */
//		case ENOENT:			//	2		/* No such file or directory */
//		case ESRCH:				//	3		/* No such process */
		case EINTR:				//	4		/* Interrupted system call */
			result = netErrUserCancel;
			break;

//		case EIO:				//	5		/* Input/output error */
//		case ENXIO:				//	6		/* Device not configured */
//		case E2BIG:				//	7		/* Argument list too long */
//		case ENOEXEC:			//	8		/* Exec format error */
//		case EBADF:				//	9		/* Bad file descriptor */
//		case ECHILD:			//	10		/* No child processes */
		case EDEADLK:			//	11		/* Resource deadlock avoided */
					/* 11 was EAGAIN */
			result = netErrWouldBlock;
			break;

		case ENOMEM:			//	12		/* Cannot allocate memory */
			result = netErrOutOfMemory;
			break;

		case EACCES:			//	13		/* Permission denied */
			result = netErrAuthFailure;
			break;

//		case EFAULT:			//	14		/* Bad address */
//		case ENOTBLK:			//	15		/* Block device required */
		case EBUSY:				//	16		/* Device busy */
			result = netErrSocketBusy;
			break;

//		case EEXIST:			//	17		/* File exists */
//		case EXDEV:				//	18		/* Cross-device link */
//		case ENODEV:			//	19		/* Operation not supported by device */
//		case ENOTDIR:			//	20		/* Not a directory */
//		case EISDIR:			//	21		/* Is a directory */
//		case EINVAL:			//	22		/* Invalid argument */
//		case ENFILE:			//	23		/* Too many open files in system */
//		case EMFILE:			//	24		/* Too many open files */
//		case ENOTTY:			//	25		/* Inappropriate ioctl for device */
//		case ETXTBSY:			//	26		/* Text file busy */
//		case EFBIG:				//	27		/* File too large */
//		case ENOSPC:			//	28		/* No space left on device */
//		case ESPIPE:			//	29		/* Illegal seek */
		case EROFS:				//	30		/* Read-only file system */
			result = netErrReadOnlySetting;
			break;

//		case EMLINK:			//	31		/* Too many links */
//		case EPIPE:				//	32		/* Broken pipe */
/* math software */
//		case EDOM:				//	33		/* Numerical argument out of domain */
//		case ERANGE:			//	34		/* Result too large */
		case EAGAIN:			//	35		/* Resource temporarily unavailable */
#if EWOULDBLOCK != EAGAIN
		case EWOULDBLOCK:		//	EAGAIN	/* Operation would block */
#endif
		case EINPROGRESS:		//	36		/* Operation now in progress */
		//	result = netErrSocketBusy;
			result = netErrWouldBlock;
			break;

/* These are defined to be the same under QNX/Neutrino */
#if EALREADY != EBUSY
		case EALREADY:			//	37		/* Operation already in progress */
			result = netErrAlreadyInProgress;
			break;
#endif
/* ipc/network software -- argument errors */
		case ENOTSOCK:			//	38		/* Socket operation on non-socket */
			result = netErrNoSocket;
			break;

		case EDESTADDRREQ:		//	39		/* Destination address required */
			result = netErrIPNoDst;
			break;

		case EMSGSIZE:			//	40		/* Message too long */
			result = netErrMessageTooBig;
			break;

//		case EPROTOTYPE:		//	41		/* Protocol wrong type for socket */
		case ENOPROTOOPT:		//	42		/* Protocol not available */
			result = netErrUnknownProtocol;
			break;

		case EPROTONOSUPPORT:	//	43		/* Protocol not supported */
			result = netErrUnknownProtocol;
			break;

		case ESOCKTNOSUPPORT:	//	44		/* Socket type not supported */
			result = netErrWrongSocketType;
			break;

		case EOPNOTSUPP:		//	45		/* Operation not supported on socket */
			result = netErrWrongSocketType;
			break;

		case EPFNOSUPPORT:		//	46		/* Protocol family not supported */
			result = netErrUnknownService;
			break;

		case EAFNOSUPPORT:		//	47		/* Address family not supported by protocol family */
			result = netErrUnknownService;
			break;

		case EADDRINUSE:		//	48		/* Address already in use */
			result = netErrPortInUse;
			break;

		case EADDRNOTAVAIL:		//	49		/* Can't assign requested address */
			result = netErrPortInUse;
			break;

/* ipc/network software -- operational errors */
		case ENETDOWN:			//	50		/* Network is down */
			result = netErrUnreachableDest;
			break;

		case ENETUNREACH:		//	51		/* Network is unreachable */
			result = netErrNoInterfaces;
			break;

		case ENETRESET:			//	52		/* Network dropped connection on reset */
		case ECONNABORTED:		//	53		/* Software caused connection abort */
		case ECONNRESET:		//	54		/* Connection reset by peer */
			result = netErrSocketClosedByRemote;
			break;

		case ENOBUFS:			//	55		/* No buffer space available */
			result = netErrNoTCB;
			break;

		case EISCONN:			//	56		/* Socket is already connected */
			result = netErrSocketAlreadyConnected;
			break;

		case ENOTCONN:			//	57		/* Socket is not connected */
			result = netErrSocketNotConnected;
			break;

		case ESHUTDOWN:			//	58		/* Can't send after socket shutdown */
			result = netErrSocketNotOpen;
			break;

//		case ETOOMANYREFS:		//	59		/* Too many references: can't splice */
		case ETIMEDOUT:			//	60		/* Connection timed out */
			result = netErrTimeout;
			break;

		case ECONNREFUSED:		//	61		/* Connection refused */
			result = netErrTimeout;
			break;

//		case ELOOP:				//	62		/* Too many levels of symbolic links */
//		case ENAMETOOLONG:		//	63		/* File name too long */
		case EHOSTDOWN:			//	64		/* Host is down */
		case EHOSTUNREACH:		//	65		/* No route to host */
			result = netErrIPNoRoute;
			break;

//		case ENOTEMPTY:			//	66		/* Directory not empty */
//		case EPROCLIM:			//	67		/* Too many processes */
//		case EUSERS:			//	68		/* Too many users */
//		case EDQUOT:			//	69		/* Disc quota exceeded */

/* Network File System */
//		case ESTALE:			//	70		/* Stale NFS file handle */
//		case EREMOTE:			//	71		/* Too many levels of remote in path */
//		case EBADRPC:			//	72		/* RPC struct is bad */
//		case ERPCMISMATCH:		//	73		/* RPC version wrong */
//		case EPROGUNAVAIL:		//	74		/* RPC prog. not avail */
//		case EPROGMISMATCH:		//	75		/* Program version wrong */
//		case EPROCUNAVAIL:		//	76		/* Bad procedure for program */
//		case ENOLCK:			//	77		/* No locks available */
//		case ENOSYS:			//	78		/* Function not implemented */
//		case EFTYPE:			//	79		/* Inappropriate file type or format */
#endif	// PLATFORM_MAC || PLATFORM_UNIX

#if PLATFORM_MAC
		case HOST_NOT_FOUND + kHErrNoBase:	//	1		/* Authoritative Answer Host not found */
			result = netErrDNSUnreachable;
			break;

		case TRY_AGAIN + kHErrNoBase:		//	2		/* Non-Authoritive Host not found, or SERVERFAIL */
			result = netErrDNSServerFailure;
			break;

		case NO_RECOVERY + kHErrNoBase:		//	3		/* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
			result = netErrDNSRefused;
			break;

		case NO_DATA + kHErrNoBase:			//	4		/* Valid name, no data record of requested type */
//		case NO_ADDRESS + kHErrNoBase:		//	4		/* no address, look for MX record */
			result = netErrDNSNonexistantName;
			break;
#endif

#if PLATFORM_WINDOWS
		case WSAEACCES:
			// (10013)
			// Permission denied.
			// An attempt was made to access a socket in a way forbidden by its access permissions. An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST).
			result = netErrWrongSocketType;	// BEST I CAN THINK OF.
			break;

		case WSAEADDRINUSE:
			// (10048)
			// Address already in use.
			// Only one usage of each socket address (protocol/IP address/port) is normally permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that wasn't closed properly, or one that is still in the process of closing. For server applications that need to bind multiple sockets to the same port number, consider using setsockopt(SO_REUSEADDR). Client applications usually need not call bind at all - connect will choose an unused port automatically. When bind is called with a wild-card address (involving ADDR_ANY), a WSAEADDRINUSE error could be delayed until the specific address is "committed." This could happen with a call to other function later, including connect, listen, WSAConnect or WSAJoinLeaf.
			result = netErrPortInUse;
			break;

		case WSAEADDRNOTAVAIL:
			// (10049)
			// Cannot assign requested address.
			// The requested address is not valid in its context. Normally results from an attempt to bind to an address that is not valid for the local machine. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote machine (e.g. address or port 0).
			result = netErrParamErr;
			break;

		case WSAEAFNOSUPPORT:
			// (10047)
			// Address family not supported by protocol family.
			// An address incompatible with the requested protocol was used. All sockets are created with an associated "address family" (i.e. AF_INET for Internet Protocols) and a generic protocol type (i.e. SOCK_STREAM). This error will be returned if an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket, e.g. in sendto.
			result = netErrParamErr;
			break;

		case WSAEALREADY:
			// (10037)
			// Operation already in progress.
			// An operation was attempted on a non-blocking socket that already had an operation in progress - i.e. calling connect a second time on a non-blocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.
			result = netErrAlreadyInProgress;
			break;

		case WSAECONNABORTED:
			// (10053)
			// Software caused connection abort.
			// An established connection was aborted by the software in your host machine, possibly due to a data transmission timeout or protocol error.
			result = netErrSocketClosedByRemote;
			break;

		case WSAECONNREFUSED:
			// (10061)
			// Connection refused.
			// No connection could be made because the target machine actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host - i.e. one with no server application running.
			result = netErrTimeout;			// As near as I can tell, if a connection is refused, NetLib merely closes the connection and eventually times out.
			break;

		case WSAECONNRESET:
			// (10054)
			// Connection reset by peer.
			// A existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, or the remote host used a "hard close" (see setsockopt for more information on the SO_LINGER option on the remote socket.) This error may also result if a connection was broken due to "keep-alive" activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET.
			result = netErrSocketClosedByRemote;
			break;

		case WSAEDESTADDRREQ:
			// (10039)
			// Destination address required.
			// A required address was omitted from an operation on a socket. For example, this error will be returned if sendto is called with the remote address of ADDR_ANY.
			result = netErrParamErr;
			break;

		case WSAEFAULT:
			// (10014)
			// Bad address.
			// The system detected an invalid pointer address in attempting to use a pointer argument of a call. This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small. For instance, if the length of an argument which is a sockaddr is smaller than sizeof(sockaddr).
			result = netErrParamErr;
			break;

		case WSAEHOSTDOWN:
			// (10064)
			// Host is down.
			// A socket operation failed because the destination host was down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT.
			result = netErrTimeout;
			break;

		case WSAEHOSTUNREACH:
			// (10065)
			// No route to host.
			// A socket operation was attempted to an unreachable host. See WSAENETUNREACH
			result = netErrUnreachableDest;
			break;

		case WSAEINPROGRESS:
			// (10036)
			// Operation now in progress.
			// A blocking operation is currently executing. Windows Sockets only allows a single blocking operation to be outstanding per task (or thread), and if any other function call is made (whether or not it references that or any other socket) the function fails with the WSAEINPROGRESS error.
			result = netErrAlreadyInProgress;
			break;

		case WSAEINTR:
			// (10004)
			// Interrupted function call.
			// A blocking operation was interrupted by a call to WSACancelBlockingCall.
			result = netErrUserCancel;
			break;

		case WSAEINVAL:
			// (10022)
			// Invalid argument.
			// Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). In some instances, it also refers to the current state of the socket - for instance, calling accept on a socket that is not listening.
			result = netErrParamErr;
			break;

		case WSAEISCONN:
			// (10056)
			// Socket is already connected.
			// A connect request was made on an already connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (For SOCK_STREAM sockets, the to parameter in sendto is ignored), although other implementations treat this as a legal occurrence.
			result = netErrSocketAlreadyConnected;
			break;

		case WSAEMFILE:
			// (10024)
			// Too many open files.
			// Too many open sockets. Each implementation may have a maximum number of socket handles available, either globally, per process or per thread.
			result = netErrNoMoreSockets;
			break;

		case WSAEMSGSIZE:
			// (10040)
			// Message too long.
			// A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself.
			result = netErrMessageTooBig;
			break;

		case WSAENETDOWN:
			// (10050)
			// Network is down.
			// A socket operation encountered a dead network. This could indicate a serious failure of the network system (i.e. the protocol stack that the WinSock DLL runs over), the network interface, or the local network itself.
			result = netErrUnreachableDest;
			break;

		case WSAENETRESET:
			// (10052)
			// Network dropped connection on reset.
			// The connection has been broken due to "keep-alive" activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed.
			result = netErrSocketClosedByRemote;
			break;

		case WSAENETUNREACH:
			// (10051)
			// Network is unreachable.
			// A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.
			result = netErrUnreachableDest;
			break;

		case WSAENOBUFS:
			// (10055)
			// No buffer space available.
			// An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
			result = netErrOutOfMemory;
			break;

		case WSAENOPROTOOPT:
			// (10042)
			// Bad protocol option.
			// An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.
			result = netErrParamErr;
			break;

		case WSAENOTCONN:
			// (10057)
			// Socket is not connected.
			// A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error - for example, setsockopt setting SO_KEEPALIVE if the connection has been reset.
			result = netErrSocketNotConnected;
			break;

		case WSAENOTSOCK:
			// (10038)
			// Socket operation on non-socket.
			// An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a valid socket, or for select, a member of an fd_set was not valid.
			result = netErrParamErr;
			break;

		case WSAEOPNOTSUPP:
			// (10045)
			// Operation not supported.
			// The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation, for example, trying to accept a connection on a datagram socket.
			result = netErrWrongSocketType;
			break;

		case WSAEPFNOSUPPORT:
			// (10046)
			// Protocol family not supported.
			// The protocol family has not been configured into the system or no implementation for it exists. Has a slightly different meaning to WSAEAFNOSUPPORT, but is interchangeable in most cases, and all Windows Sockets functions that return one of these specify WSAEAFNOSUPPORT.
			result = netErrParamErr;
			break;

		case WSAEPROCLIM:
			// (10067)
			// Too many processes.
			// A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously. WSAStartup may fail with this error if the limit has been reached.
			result = netErrOutOfResources;
			break;

		case WSAEPROTONOSUPPORT:
			// (10043)
			// Protocol not supported.
			// The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.
			result = netErrUnknownProtocol;
			break;

		case WSAEPROTOTYPE:
			// (10041)
			// Protocol wrong type for socket.
			// A protocol was specified in the socket function call that does not support the semantics of the socket type requested. For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM.
			result = netErrUnknownProtocol;
			break;

		case WSAESHUTDOWN:
			// (10058)
			// Cannot send after socket shutdown.
			// A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving or both has been discontinued.
			result = netErrNotOpen;
			break;

		case WSAESOCKTNOSUPPORT:
			// (10044)
			// Socket type not supported.
			// The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.
			result = netErrParamErr;
			break;

		case WSAETIMEDOUT:
			// (10060)
			// Connection timed out.
			// A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.
			result = netErrTimeout;
			break;

//		case WSATYPE_NOT_FOUND:
			// (10109)
			// Class type not found.
			// The specified class was not found.

		case WSAEWOULDBLOCK:
			// (10035)
			// Resource temporarily unavailable.
			// This error is returned from operations on non-blocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a non-fatal error, and the operation should be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect on a non-blocking SOCK_STREAM socket, since some time must elapse for the connection to be established.
			result = netErrWouldBlock;
			break;

		case WSAHOST_NOT_FOUND:
			// (11001)
			// Host not found.
			// No such host is known. The name is not an official hostname or alias, or it cannot be found in the database(s) being queried. This error may also be returned for protocol and service queries, and means the specified name could not be found in the relevant database.
			result = netErrDNSNonexistantName;
			break;

//		case WSA_INVALID_HANDLE:
			// (OS dependent)
			// Specified event object handle is invalid.
			// An application attempts to use an event object, but the specified handle is not valid.

//		case WSA_INVALID_PARAMETER:
			// (OS dependent)
			// One or more parameters are invalid.
			// An application used a Windows Sockets function which directly maps to a Win32 function. The Win32 function is indicating a problem with one or more parameters.

//		case WSAINVALIDPROCTABLE:
			// (OS dependent)
			// Invalid procedure table from service provider.
			// A service provider returned a bogus proc table to WS2_32.DLL. (Usually caused by one or more of the function pointers being NULL.)

//		case WSAINVALIDPROVIDER:
			// (OS dependent)
			// Invalid service provider version number.
			// A service provider returned a version number other than 2.0.

//		case WSA_IO_INCOMPLETE:
			// (OS dependent)
			// Overlapped I/O event object not in signaled state.
			// The application has tried to determine the status of an overlapped operation which is not yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set to false) in a polling mode to determine when an overlapped operation has completed will get this error code until the operation is complete.

//		case WSA_IO_PENDING:
			// (OS dependent)
			// Overlapped operations will complete later.
			// The application has initiated an overlapped operation which cannot be completed immediately. A completion indication will be given at a later time when the operation has been completed.

//		case WSA_NOT_ENOUGH_MEMORY:
			// (OS dependent)
			// Insufficient memory available.
			// An application used a Windows Sockets function which directly maps to a Win32 function. The Win32 function is indicating a lack of required memory resources.

		case WSANOTINITIALISED:
			// (10093)
			// Successful WSAStartup not yet performed.
			// Either the application hasn't called WSAStartup or WSAStartup failed. The application may be accessing a socket which the current active task does not own (i.e. trying to share a socket between tasks), or WSACleanup has been called too many times.
			result = netErrNotOpen;
			break;

		case WSANO_DATA:
			// (11004)
			// Valid name, no data record of requested type.
			// The requested name is valid and was found in the database, but it does not have the correct associated data being resolved for. The usual example for this is a hostname -> address translation attempt (using gethostbyname or WSAAsyncGetHostByName) which uses the DNS (Domain Name Server), and an MX record is returned but no A record - indicating the host itself exists, but is not directly reachable.
			result = netErrDNSFormat;
			break;

		case WSANO_RECOVERY:
			// (11003)
			// This is a non-recoverable error.
			// This indicates some sort of non-recoverable error occurred during a database lookup. This may be because the database files (e.g. BSD-compatible HOSTS, SERVICES or PROTOCOLS files) could not be found, or a DNS request was returned by the server with a severe error.
			result = netErrInternal;
			break;

//		case WSAPROVIDERFAILEDINIT:
			// (OS dependent)
			// Unable to initialize a service provider.
			// Either a service provider's DLL could not be loaded (LoadLibrary failed) or the provider's WSPStartup/NSPStartup function failed.

//		case WSASYSCALLFAILURE:
			// (OS dependent)
			// System call failure.
			// Returned when a system call that should never fail does. For example, if a call to WaitForMultipleObjects fails or one of the registry functions fails trying to manipulate theprotocol/namespace catalogs.

		case WSASYSNOTREADY:
			// (10091)
			// Network subsystem is unavailable.
			// This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. Users should check:
			// * that the appropriate Windows Sockets DLL file is in the current path,
			// * that they are not trying to use more than one Windows Sockets implementation simultaneously. If there is more than one WINSOCK DLL on your system, be sure the first one in the path is appropriate for the network subsystem currently loaded.
			// * the Windows Sockets implementation documentation to be sure all necessary components are currently installed and configured correctly.
			result = netErrInternal;
			break;

		case WSATRY_AGAIN:
			// (11002)
			// Non-authoritative host not found.
			// This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server. A retry at some time later may be successful.
			result = netErrDNSTimeout;
			break;

		case WSAVERNOTSUPPORTED:
			// (10092)
			// WINSOCK.DLL version out of range.
			// The current Windows Sockets implementation does not support the Windows Sockets specification version requested by the application. Check that no old Windows Sockets DLL files are being accessed.
			result = netErrInternal;
			break;

		case WSAEDISCON:
			// (10094)
			// Graceful shutdown in progress.
			// Returned by WSARecv and WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.
			result = netErrSocketNotConnected;
			break;

//		case WSA_OPERATION_ABORTED:
			// (OS dependent)
			// Overlapped operation aborted.
			// An overlapped operation was canceled due to the closure of the socket, or the execution of the SIO_FLUSH command in WSAIoctl.
#endif	// PLATFORM_WINDOWS
`;

let namesHost = [];
let caseParsed = false;

for (const line of code.split('\n')) {
    let match;

    if (!caseParsed && (match = line.match(/^\s*case\s+(\w+)\s*:/))) {
        namesHost.push(match[1]);
    } else if (!caseParsed && namesHost.length > 0 && (match = line.match(/^\s*result\s*=\s*(\w+)\s*;/))) {
        const namePalm = match[1];
        caseParsed = true;

        namesHost.forEach((nameHost) => console.log(`    '${nameHost}': ${namePalm},`));
    } else if (line.match(/^\s*break\s*;/)) {
        caseParsed = false;
        namesHost = [];
    }
}
