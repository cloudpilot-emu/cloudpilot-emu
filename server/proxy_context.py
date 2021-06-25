import asyncio
import logging
import select
import socket

import hexdump
import websockets

import net_errors as err
import proto.networking_pb2 as networking
from logger import debug, error, info, logger, warning
from socket_context import MAX_TIMEOUT, SocketContext

MAX_HANDLE = 31

SOCKET_TYPE = {
    1: socket.SOCK_STREAM,
    2: socket.SOCK_DGRAM,
    3: socket.SOCK_RAW
}


class InvalidHandleError(Exception):
    def __init__(self, handle):
        super().__init__()
        self.handle = handle


class InvalidAddressError(Exception):
    def __init__(self, address):
        super().__init__()
        self.address = address


class NoMoreSocketsError(Exception):
    pass


class BadPacketException(Exception):
    pass


def deserializeAddress(addr):
    return(
        f'{(addr.ip >> 24) & 0xff}.{(addr.ip >> 16) & 0xff}.{(addr.ip >> 8) & 0xff}.{addr.ip & 0xff}', addr.port)


def serializeIp(addr):
    try:
        parts = [int(x) for x in addr.split(".")]
    except Exception:
        warning(f'address is not IPv4 {addr}')
        return None

    if len(parts) != 4:
        warning(f'address is not IPv4 {addr}')
        return None

    return ((parts[0] << 24) | (parts[1] << 16) | (
        parts[2] << 8) | parts[3]) & 0xffffffff


def serializeAddress(addr, target):

    if type(addr) != tuple or len(addr) != 2 or not isinstance(addr[0], str) or not isinstance(addr[1], int):
        target.port = 0
        target.ip = 0

        warning(f'address is not IPv4 {addr}')

        return

    target.port = addr[1]
    target.ip = serializeIp(addr[0])


def translateFlags(flags):
    translatedFlags = 0

    if flags & 0x01:
        translatedFlags |= socket.MSG_OOB

    if flags & 0x02:
        translatedFlags |= socket.MSG_PEEK

    if flags & 0x04:
        translatedFlags |= socket.MSG_DONTROUTE

    return flags


def formatException(ex):
    if isinstance(ex, InvalidHandleError):
        return f'invalid handle {ex.handle}'

    if isinstance(ex, InvalidAddressError):
        return f'invalid address {ex.address}'

    if isinstance(ex, BadPacketException):
        return 'bad IPv4 packet'

    if isinstance(ex, NoMoreSocketsError):
        return 'no more sockets'

    return f'{type(ex).__name__}: {ex}'


def exceptionToErr(ex):
    if isinstance(ex, socket.gaierror):
        return err.herrnoToPalm(ex.args[0])

    if isinstance(ex, OSError):
        return err.errnoToPalm(ex.errno)

    if isinstance(ex, InvalidHandleError):
        return err.netErrParamErr

    if isinstance(ex, InvalidAddressError):
        return err.netErrInternal

    if isinstance(ex, BadPacketException):
        return err.netErrParamErr

    if isinstance(ex, NoMoreSocketsError):
        return err.netErrNoMoreSockets

    return err.netErrInternal


def ipFromIpPacket(packet):
    if len(packet) < 20:
        raise BadPacketException()

    return f'{packet[16]}.{packet[17]}.{packet[18]}.{packet[19]}'


def removeIpHeader(packet, socketCtx):
    if socketCtx.type != socket.SOCK_RAW or len(packet) < 20 or (packet[0] >> 4) != 4 or (packet[0] & 0x0f) < 5 or packet[9] != 1:
        return packet

    hlen = packet[0] & 0x0f
    return packet[4*hlen:]


def logPayload(data):
    if not logger.isEnabledFor(logging.DEBUG):
        return

    dump = ""
    for line in hexdump.dumpgen(data):
        dump += ("\n" + line)

    debug(dump)


class ProxyContext:
    connectionIndex = 0

    def __init__(self):
        self.echoRequest = None
        self._sockets = [None] * MAX_HANDLE

    async def start(self, ws):
        info(f'starting proxy connection {self.connectionIndex}')
        self.connectionIndex = self.connectionIndex + 1

        self._ws = ws

        try:
            while True:
                await self._handleMessage(await ws.recv())

        except (websockets.exceptions.ConnectionClosedError, websockets.exceptions.ConnectionClosedOK):
            info(f'connection {self.connectionIndex} closed')

        await self._closeAllSockets()

    async def _handleMessage(self, message):
        request = networking.MsgRequest()
        request.ParseFromString(message)
        response = None

        requestType = request.WhichOneof("payload")

        if requestType == "socketOpenRequest":
            response = await self._handleSocketOpen(request.socketOpenRequest)

        elif requestType == "socketBindRequest":
            response = await self._handleSocketBind(request.socketBindRequest)

        elif requestType == "socketAddrRequest":
            response = await self._handleSocketAddr(request.socketAddrRequest)

        elif requestType == "socketSendRequest":
            response = await self._handleSocketSend(request.socketSendRequest)

        elif requestType == "socketReceiveRequest":
            response = await self._handeSocketReceive(request.socketReceiveRequest)

        elif requestType == "socketCloseRequest":
            response = await self._handleSocketClose(request.socketCloseRequest)

        elif requestType == "getHostByNameRequest":
            response = await self._handleGetHostByName(request.getHostByNameRequest)

        elif requestType == "getServByNameRequest":
            response = await self._handleGetServByName(request.getServByNameRequest)

        elif requestType == "socketConnectRequest":
            response = await self._handleSocketConnect(request.socketConnectRequest)

        elif requestType == "selectRequest":
            response = await self._handleSelect(request.selectRequest)

        else:
            response = networking.MsgResponse()
            response.invalidRequestResponse.tag = True

            error(f'unknown request {requestType}')

        if response:
            response.id = request.id

            await self._ws.send(response.SerializeToString())

    async def _handleSocketOpen(self, request):
        debug(
            f'socketOpenRequest: type={request.type} protocol={request.protocol}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketOpenResponse

        response.handle = -1

        if not request.type in SOCKET_TYPE:
            response.err = err.netErrParamErr
            return responseMsg

        try:
            sockType = SOCKET_TYPE[request.type]
            sockProtocol = 0

            if sockType == socket.SOCK_RAW:
                if request.protocol == 255 or request.protocol == 1:
                    sockProtocol = socket.IPPROTO_ICMP
                else:
                    error(
                        f'unsupported protocol for RAW socket: {request.protocol}')
                    response.err = err.netErrParamErr

                    return responseMsg

            handle = self._getFreeHandle()

            sock = await asyncio.to_thread(lambda: socket.socket(
                socket.AF_INET, sockType, sockProtocol))

            self._sockets[handle] = SocketContext(
                sock, sockType)

            response.handle = handle
            response.err = 0

        except Exception as ex:
            error(f'failed to open socket: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketBind(self, request):
        debug(
            f'socketBindRequest: handle={request.handle} address={deserializeAddress(request.address)} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketBindResponse

        try:
            socketCtx = self._getSocketCtx(request.handle)

            socketCtx.setTimeoutMsec(request.timeout)

            await asyncio.to_thread(lambda: socketCtx.socket.bind(
                deserializeAddress(request.address)))
            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            error(f'failed to bind socket: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketAddr(self, request):
        debug(
            f'socketAddrRequest: handle={request.handle} requestAddressLocal={request.requestAddressLocal} requestAddressRemote={request.requestAddressRemote} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketAddrResponse

        try:
            socketCtx = self._getSocketCtx(request.handle)
            sock = socketCtx.socket

            socketCtx.setTimeoutMsec(request.timeout)

            if request.requestAddressLocal:
                serializeAddress(await asyncio.to_thread(lambda: sock.getsockname()), response.addressLocal)
                socketCtx.updateTimeout()

            if request.requestAddressRemote:
                serializeAddress(await asyncio.to_thread(lambda: sock.getpeername()), response.addressRemote)

            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            error(
                f'failed to get socket addresses: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketSend(self, request):
        debug(
            f'socketSendRequest: handle={request.handle} len={len(request.data)} flags={request.flags} timeout={request.timeout} {f"address={deserializeAddress(request.address)}" if request.HasField("address") else ""}')

        logPayload(request.data)

        self.echoRequest = request.data

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketSendResponse
        response.bytesSent = -1

        try:
            socketCtx = self._getSocketCtx(request.handle)
            sock = socketCtx.socket
            flags = translateFlags(request.flags)

            socketCtx.setTimeoutMsec(request.timeout)

            if socketCtx.type == socket.SOCK_RAW:
                data = removeIpHeader(request.data, socketCtx)

                address = deserializeAddress(request.address) if request.HasField(
                    "address") else (ipFromIpPacket(request.data), 1)

                response.bytesSent = await asyncio.to_thread(lambda: sock.sendto(data, flags, address) + len(request.data) - len(data))

            elif request.HasField("address"):
                response.bytesSent = await asyncio.to_thread(lambda: sock.sendto(request.data, flags, deserializeAddress(request.address)))

            else:
                response.bytesSent = await asyncio.to_thread(lambda: sock.send(request.data, flags))

            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            error(f'failed to send: {formatException(ex)}')

            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handeSocketReceive(self, request):
        debug(
            f'socketReceiveRequest: handle={request.handle} flags={request.flags} timeout={request.timeout} maxLength={request.maxLen}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketReceiveResponse

        response.data = b''

        try:
            socketCtx = self._getSocketCtx(request.handle)
            flags = translateFlags(request.flags)

            socketCtx.setTimeoutMsec(request.timeout)
            data, address = await asyncio.to_thread(lambda: socketCtx.socket.recvfrom(request.maxLen, flags))

            if address != None:
                serializeAddress(address, response.address)

            response.data = data
            response.err = 0

            logPayload(response.data)

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            error(f'failed to receive: {formatException(ex)}')

            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketClose(self, request):
        debug(
            f'socketCloseRequest: handle={request.handle} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketCloseResponse

        try:
            socketContext = self._getSocketCtx(request.handle)
            self._sockets[request.handle] = None

            socketContext.setTimeoutMsec(request.timeout)

            await socketContext.close()
            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            error(
                f'sfailed to close socket {request.handle}: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleGetHostByName(self, request):
        debug(f'getHostByNameRequest name{request.name}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.getHostByNameResponse
        response.name = ""

        try:
            hostname, aliases, addresses = await asyncio.to_thread(lambda: socket.gethostbyname_ex(
                request.name))

            response.name = hostname

            if len(aliases) > 0:
                response.alias = aliases[0]

            response.addresses[:] = [
                serializeIp(x) for x in addresses if serializeIp(x) != None][:3]

            response.err = 0

        except Exception as ex:
            error(
                f'failed to resolve host {request.name}: {formatException(ex)}')

            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleGetServByName(self, request):
        debug(
            f'getServByNameRequest name={request.name} protocol={request.protocol}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.getServByNameResponse
        response.port = 0

        try:
            response.port = await asyncio.to_thread(lambda: socket.getservbyname(
                request.name, request.protocol))

            response.err = 0

        except Exception as ex:
            error(f'failed to resolve service: {formatException(ex)}')

            response.err = err.netErrUnknownService

        return responseMsg

    async def _handleSocketConnect(self, request):
        debug(
            f'socketConnectRequest handle={request.handle} address={deserializeAddress(request.address)} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketConnectResponse

        try:
            socketContext = self._getSocketCtx(request.handle)

            socketContext.setTimeoutMsec(request.timeout)

            await asyncio.to_thread(lambda: socketContext.socket.connect(deserializeAddress(request.address)))

            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            error(f'failed to connect socket: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSelect(self, request):
        debug(f'select width={request.width} readFDs={self._fdSetToHandles(request.readFDs, request.width)} writeFDs={self._fdSetToHandles(request.writeFDs, request.width)} exceptFDs={self._fdSetToHandles(request.exceptFDs, request.width)} timeout={request.timeout}')

        try:
            responseMsg = networking.MsgResponse()
            response = responseMsg.selectResponse

            response.readFDs = 0
            response.writeFDs = 0
            response.exceptFDs = 0
            response.err = err.netErrInternal

            timeout = min(MAX_TIMEOUT, request.timeout /
                          1000 if request.timeout >= 0 else MAX_TIMEOUT)

            rlist, wlist, xlist = await asyncio.to_thread(
                lambda: select.select(
                    self._fdSetToSockets(request.readFDs, request.width),
                    self._fdSetToSockets(request.writeFDs, request.width),
                    self._fdSetToSockets(request.exceptFDs, request.width),
                    timeout
                )
            )

            response.readFDs = self._socketsToFdSet(rlist)
            response.writeFDs = self._socketsToFdSet(wlist)
            response.exceptFDs = self._socketsToFdSet(xlist)

            response.err = 0

        except Exception as ex:
            error(f'select failed {ex}')

        return responseMsg

    async def _closeAllSockets(self):
        async def closeCtx(ctx):
            try:
                ctx.setTimeoutMsec(None)

                await ctx.close()
            except Exception as ex:
                error(
                    f'failed to close socket: {formatException(ex)}')

        contexts = [ctx for ctx in self._sockets if ctx != None]

        if len(contexts) > 0:
            await asyncio.wait([asyncio.create_task(closeCtx(ctx)) for ctx in contexts])

    def _getSocketCtx(self, handle):
        if handle < 1 or handle > MAX_HANDLE or self._sockets[handle] == None:
            raise InvalidHandleError(handle)

        return self._sockets[handle]

    def _getFreeHandle(self):
        for handle, ctx in enumerate(self._sockets):
            if handle > 0 and ctx == None:
                return handle

        raise NoMoreSocketsError()

    def _fdSetToHandles(self, fds, width):
        width = max(min(width, 32), 0)

        return [handle for handle in range(1, width) if (fds & (1 << handle) > 0)]

    def _fdSetToSockets(self, fds, width):
        return [self._sockets[handle].socket for handle in self._fdSetToHandles(fds, width) if self._sockets[handle] != None]

    def _socketsToFdSet(self, sockets):
        packed = 0

        for handle, ctx in enumerate(self._sockets):
            if ctx == None or not ctx.socket in sockets:
                continue

            packed |= 1 << handle

        return packed
