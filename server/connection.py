import asyncio
import errno
import logging
import select
import socket
from struct import unpack
from typing import Awaitable, List, Optional, Tuple, cast

import dns.resolver
import hexdump  # type: ignore
import net_errors as err
import proto.networking_pb2 as networking
import sockopt
from aiohttp import WSMsgType, web
from aiohttp.web_ws import WebSocketResponse
from logger import debug, error, info, logger, warning
from palmos_constants import *
from socket_context import MAX_TIMEOUT, SocketContext
from util import runInThread

MAX_HANDLE = 31

SOCKET_TYPE = {
    1: socket.SOCK_STREAM,
    2: socket.SOCK_DGRAM,
    3: socket.SOCK_RAW
}


class InvalidHandleError(Exception):
    handle: int

    def __init__(self, handle: int):
        super().__init__()
        self.handle = handle


class InvalidAddressError(Exception):
    address: int

    def __init__(self, address: int):
        super().__init__()
        self.address = address


class NoMoreSocketsError(Exception):
    pass


class BadPacketException(Exception):
    pass


def deserializeAddress(addr: networking.Address) -> Tuple[str, int]:
    return (
        f'{(addr.ip >> 24) & 0xff}.{(addr.ip >> 16) & 0xff}.{(addr.ip >> 8) & 0xff}.{addr.ip & 0xff}', addr.port)


def serializeIp(addr: str) -> Optional[int]:
    try:
        parts = [int(x) for x in addr.split(".")]
    except Exception:
        return None

    if len(parts) != 4:
        return None

    return ((parts[0] << 24) | (parts[1] << 16) | (
        parts[2] << 8) | parts[3]) & 0xffffffff


def serializeAddress(addr: Tuple[str, int], target: networking.Address):
    if type(addr) != tuple or len(addr) != 2 or not isinstance(addr[0], str) or not isinstance(addr[1], int):
        target.port = 0
        target.ip = 0

        warning(f'address is not IPv4 {addr}')

        return

    ip = serializeIp(addr[0])

    if ip is not None:
        target.port = addr[1]
        target.ip = ip
    else:
        target.port = 0
        target.ip = 0


def translateFlags(flags: int) -> int:
    translatedFlags = 0

    if flags & 0x01:
        translatedFlags |= socket.MSG_OOB

    if flags & 0x02:
        translatedFlags |= socket.MSG_PEEK

    if flags & 0x04:
        translatedFlags |= socket.MSG_DONTROUTE

    return flags


def shutdownHow(direction: int) -> int:
    if direction == netSocketDirOutput:
        return socket.SHUT_WR

    if direction == netSocketDirInput:
        return socket.SHUT_RD

    if direction == netSocketDirBoth:
        return socket.SHUT_RDWR

    return 0


def formatException(ex: Exception) -> str:
    if isinstance(ex, InvalidHandleError):
        return f'invalid handle {ex.handle}'

    if isinstance(ex, InvalidAddressError):
        return f'invalid address {ex.address}'

    if isinstance(ex, BadPacketException):
        return 'bad IPv4 packet'

    if isinstance(ex, NoMoreSocketsError):
        return 'no more sockets'

    return f'{type(ex).__name__}: {ex}'


def exceptionToErr(ex: Exception) -> int:
    if isinstance(ex, socket.timeout):
        return err.netErrTimeout

    if isinstance(ex, socket.gaierror):
        return err.gaierrnoToPalm(ex.args[0])

    if isinstance(ex, socket.herror):
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


def logAndConvertException(msg: str, ex: Exception) -> int:
    ecode = exceptionToErr(ex)

    if ecode != err.netErrTimeout and ecode != err.netErrWouldBlock:
        warning(f'{msg}: {formatException(ex)}')

    return ecode


def ipFromIpPacket(packet: bytes) -> str:
    if len(packet) < 20:
        raise BadPacketException()

    return f'{packet[16]}.{packet[17]}.{packet[18]}.{packet[19]}'


def removeIpHeader(packet: bytes, socketCtx: SocketContext) -> bytes:
    if socketCtx.type != socket.SOCK_RAW or len(packet) < 20 or (packet[0] >> 4) != 4 or (packet[0] & 0x0f) < 5 or packet[9] != 1:
        return packet

    hlen = packet[0] & 0x0f
    return packet[4*hlen:]


def logPayload(data: bytes):
    if not logger.isEnabledFor(logging.DEBUG):
        return

    dump = ""
    for line in hexdump.dumpgen(data):
        dump += ("\n" + line)

    debug(dump)


class Connection:
    nextConnectionIndex: int = 0

    connectionIndex: int

    _sockets: List[Optional[SocketContext]]
    _forceBindAddress: Optional[Tuple[str, int]]
    _nameserver: Optional[int]

    def __init__(self, forceBindAddress: Optional[str] = None, nameserver: Optional[int] = None):
        self._sockets = [None] * MAX_HANDLE
        self.connectionIndex = Connection.nextConnectionIndex

        self._forceBindAddress = (
            forceBindAddress, 0) if forceBindAddress is not None else None

        self._nameserver = nameserver

        Connection.nextConnectionIndex += 1

    async def handle(self, ws: web.WebSocketResponse):
        info(f'starting proxy connection {self.connectionIndex}')

        try:
            async for message in ws:
                if message.type == WSMsgType.BINARY:
                    await self._handleMessage(message.data, ws)

                elif message.type == WSMsgType.ERROR:
                    error(
                        f"proxy connection closed with exception {ws.exception()}")

                else:
                    error("bad websocket message")

            info(f'connection {self.connectionIndex} closed')

        finally:
            await self._closeAllSockets()

    async def _handleMessage(self, message: bytes, ws: WebSocketResponse):
        request = networking.MsgRequest()
        request.ParseFromString(message)

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

        elif requestType == "settingGetRequest":
            response = await self._handleSettingGet(request.settingGetRequest)

        elif requestType == "socketOptionSetRequest":
            response = await self._handleSocketOptionSet(request.socketOptionSetRequest)

        elif requestType == "socketOptionGetRequest":
            response = await self._handleSocketOptionGet(request.socketOptionGetRequest)

        elif requestType == "socketListenRequest":
            response = await self._handleSocketListen(request.socketListenRequest)

        elif requestType == "socketAcceptRequest":
            response = await self._handleSocketAccept(request.socketAcceptRequest)

        elif requestType == "socketShutdownRequest":
            response = await self._handleSocketShutdown(request.socketShutdownRequest)

        else:
            response = networking.MsgResponse()
            response.invalidRequestResponse.tag = True

            error(f'unknown request {requestType}')

        response.id = request.id

        await ws.send_bytes(response.SerializeToString())

    async def _handleSocketOpen(self, request: networking.MsgSocketOpenRequest) -> networking.MsgResponse:
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
                    warning(
                        f'unsupported protocol for RAW socket: {request.protocol}')
                    response.err = err.netErrParamErr

                    return responseMsg

            handle = self._getFreeHandle()

            sock = await runInThread(lambda: socket.socket(
                socket.AF_INET, sockType, sockProtocol))

            self._sockets[handle] = SocketContext(
                sock, sockType)

            response.handle = handle
            response.err = 0

        except Exception as ex:
            response.err = logAndConvertException("failed to open socket", ex)

        return responseMsg

    async def _handleSocketBind(self, request: networking.MsgSocketBindRequest) -> networking.MsgResponse:
        debug(
            f'socketBindRequest: handle={request.handle} address={deserializeAddress(request.address)} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketBindResponse

        try:
            socketCtx = self._getSocketCtx(request.handle)

            socketCtx.setTimeoutMsec(request.timeout)

            await runInThread(lambda: socketCtx.socket.bind(
                deserializeAddress(request.address) if self._forceBindAddress is None else self._forceBindAddress))

            socketCtx.bound = True
            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            response.err = logAndConvertException("failed to bind socket", ex)

        return responseMsg

    async def _handleSocketAddr(self, request: networking.MsgSocketAddrRequest) -> networking.MsgResponse:
        debug(
            f'socketAddrRequest: handle={request.handle} requestAddressLocal={request.requestAddressLocal} requestAddressRemote={request.requestAddressRemote} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketAddrResponse

        try:
            socketCtx = self._getSocketCtx(request.handle)
            sock = socketCtx.socket

            socketCtx.setTimeoutMsec(request.timeout)

            if request.requestAddressLocal:
                try:
                    serializeAddress(await runInThread(lambda: sock.getsockname()), response.addressLocal)
                except OSError as ex:
                    if ex.errno in errno.errorcode and errno.errorcode[ex.errno] == "WSAEINVAL":
                        response.addressLocal.port = 0
                        response.addressLocal.ip = 0
                    else:
                        raise ex

                socketCtx.updateTimeout()

            if request.requestAddressRemote:
                serializeAddress(await runInThread(lambda: sock.getpeername()), response.addressRemote)

            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            response.err = logAndConvertException(
                "failed to get socket addresses", ex)

        return responseMsg

    async def _handleSocketSend(self, request: networking.MsgSocketSendRequest) -> networking.MsgResponse:
        debug(
            f'socketSendRequest: handle={request.handle} len={len(request.data)} flags={request.flags} timeout={request.timeout} {f"address={deserializeAddress(request.address)}" if request.HasField("address") else ""}')

        logPayload(request.data)

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

                response.bytesSent = await runInThread(lambda: sock.sendto(data, flags, address) + len(request.data) - len(data))

            elif request.HasField("address"):
                response.bytesSent = await runInThread(lambda: sock.sendto(request.data, flags, deserializeAddress(request.address)))

            else:
                response.bytesSent = await runInThread(lambda: sock.send(request.data, flags))

            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            response.err = logAndConvertException("failed to send", ex)

        return responseMsg

    async def _handeSocketReceive(self, request: networking.MsgSocketReceiveRequest) -> networking.MsgResponse:
        debug(
            f'socketReceiveRequest: handle={request.handle} flags={request.flags} timeout={request.timeout} maxLength={request.maxLen} addressRequested={request.addressRequested}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketReceiveResponse

        response.data = b''

        try:
            socketCtx = self._getSocketCtx(request.handle)
            flags = translateFlags(request.flags)

            socketCtx.setTimeoutMsec(request.timeout)
            data, address = await runInThread(lambda: socketCtx.socket.recvfrom(request.maxLen, flags))

            if address != None and request.addressRequested:
                serializeAddress(address, response.address)

            response.data = data
            response.err = 0

            logPayload(response.data)

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            response.err = logAndConvertException("failed to receive", ex)

        return responseMsg

    async def _handleSocketClose(self, request: networking.MsgSocketCloseRequest) -> networking.MsgResponse:
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
            response.err = logAndConvertException(
                f'failed to close socket {request.handle}', ex)

        return responseMsg

    async def _handleGetHostByName(self, request: networking.MsgGetHostByNameRequest) -> networking.MsgResponse:
        debug(f'getHostByNameRequest name={request.name}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.getHostByNameResponse
        response.name = ""

        try:
            hostname, aliases, addresses = await runInThread(lambda: socket.gethostbyname_ex(
                request.name))

            response.name = hostname

            if len(aliases) > 0:
                response.alias = aliases[0]

            response.addresses[:] = [
                cast(int, serializeIp(x)) for x in addresses if serializeIp(x) is not None][:3]

            response.err = 0

        except Exception as ex:
            response.err = logAndConvertException(
                f'failed to resolve host {request.name}', ex)

        return responseMsg

    async def _handleGetServByName(self, request: networking.MsgGetServByNameRequest) -> networking.MsgResponse:
        debug(
            f'getServByNameRequest name={request.name} protocol={request.protocol}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.getServByNameResponse
        response.port = 0

        try:
            response.port = await runInThread(lambda: socket.getservbyname(
                request.name, request.protocol))

            response.err = 0

        except Exception as ex:
            info(f'failed to resolve service: {formatException(ex)}')

            response.err = err.netErrUnknownService

        return responseMsg

    async def _handleSocketConnect(self, request: networking.MsgSocketConnectRequest) -> networking.MsgResponse:
        debug(
            f'socketConnectRequest handle={request.handle} address={deserializeAddress(request.address)} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketConnectResponse

        try:
            socketContext = self._getSocketCtx(request.handle)

            socketContext.setTimeoutMsec(request.timeout)

            if self._forceBindAddress is not None and not socketContext.bound:
                forceBindAddress = self._forceBindAddress

                await runInThread(lambda: socketContext.socket.bind(forceBindAddress))

                socketContext.bound = True
                socketContext.updateTimeout()

            await runInThread(lambda: socketContext.socket.connect(deserializeAddress(request.address)))

            response.err = 0

        except socket.timeout:
            response.err = err.netErrTimeout

        except Exception as ex:
            response.err = logAndConvertException(
                'failed to connect socket', ex)

        return responseMsg

    async def _handleSelect(self, request: networking.MsgSelectRequest) -> networking.MsgResponse:
        debug(f'select width={request.width} readFDs={self._fdSetToHandles(request.readFDs, request.width)} writeFDs={self._fdSetToHandles(request.writeFDs, request.width)} exceptFDs={self._fdSetToHandles(request.exceptFDs, request.width)} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.selectResponse

        try:
            response.readFDs = 0
            response.writeFDs = 0
            response.exceptFDs = 0
            response.err = err.netErrInternal

            timeout = min(MAX_TIMEOUT, request.timeout /
                          1000 if request.timeout >= 0 else MAX_TIMEOUT)

            rlist, wlist, xlist = await runInThread(
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
            response.err = logAndConvertException('select failed', ex)

        return responseMsg

    async def _handleSettingGet(self, request: networking.MsgSettingGetRequest) -> networking.MsgResponse:
        debug(f'settingGet setting={request.setting}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.settingGetResponse
        response.err = 0

        try:
            if request.setting == netSettingHostName:
                response.strval = socket.gethostname()

            elif request.setting in [netSettingPrimaryDNS, netSettingSecondaryDNS, netSettingRTPrimaryDNS, netSettingRTSecondaryDNS]:
                resolver = dns.resolver.Resolver()

                if self._nameserver is None:
                    for nameserver in resolver.nameservers:
                        ip = serializeIp(str(nameserver))

                        if ip != None:
                            break
                else:
                    ip = self._nameserver

                response.uint32val = ip if ip is not None else 0x08080808

            else:
                response.err = err.netErrParamErr

        except Exception as ex:
            response.err = logAndConvertException('settingGet failed', ex)

        return responseMsg

    async def _handleSocketOptionSet(self, request: networking.MsgSocketOptionSetRequest) -> networking.MsgResponse:
        value = sockopt.sockoptValue(request)

        debug(
            f'socketOptionSet handle={request.handle} level={request.level} option={request.option} value={value} timeout={request.timeout}')  # type: ignore

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketOptionSetResponse
        response.err = 0

        try:
            socketCtx = self._getSocketCtx(request.handle)
            socket = socketCtx.socket

            if request.level == netSocketOptLevelSocket and request.option == netSocketOptSockNonBlocking:
                await runInThread(lambda: socketCtx.setNonblocking(bool(value)))
            else:
                level = sockopt.translateSockoptLevel(request.level)
                option = sockopt.translateSockoptOption(
                    request.level, request.option)

                if level is None or option is None:
                    response.err = err.netErrParamErr
                else:
                    socketCtx.setTimeoutMsec(request.timeout)

                    # https://github.com/python/mypy/issues/4297
                    _level = level
                    _option = option

                    await runInThread(lambda: socket.setsockopt(_level, _option, value))

        except Exception as ex:
            response.err = logAndConvertException(
                f'socketOptionSet failed handle={request.handle}', ex)

        return responseMsg

    async def _handleSocketOptionGet(self, request: networking.MsgSocketOptionGetRequest) -> networking.MsgResponse:
        debug(
            f'socketOptionGet handle={request.handle} level={request.level} option={request.option} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketOptionGetResponse
        response.err = 0

        try:
            socketCtx = self._getSocketCtx(request.handle)
            socket = socketCtx.socket

            level = sockopt.translateSockoptLevel(request.level)
            option = sockopt.translateSockoptOption(
                request.level, request.option)

            if request.level == netSocketOptLevelSocket and request.option == netSocketOptSockNonBlocking:
                response.intval = socketCtx.getNonblocking()

            elif level == None or option == None:
                response.err = err.netErrParamErr

            elif request.level == netSocketOptLevelSocket and request.option == netSocketOptSockLinger:
                socketCtx.setTimeoutMsec(request.timeout)

                # https://github.com/python/mypy/issues/4297
                assert level is not None and option is not None
                _level = level
                _option = option

                (onoff, linger) = unpack('ii', await runInThread(lambda: socket.getsockopt(_level, _option, 8)))

                response.intval = (onoff & 0xffff) | ((linger & 0xffff) << 16)

            elif request.level == netSocketOptLevelIP:
                socketCtx.setTimeoutMsec(request.timeout)

                # https://github.com/python/mypy/issues/4297
                assert level is not None and option is not None
                _level = level
                _option = option

                response.bufval = await runInThread(lambda: socket.getsockopt(_level, _option, 40))

            else:
                socketCtx.setTimeoutMsec(request.timeout)

                # https://github.com/python/mypy/issues/4297
                assert level is not None and option is not None
                _level = level
                _option = option

                response.intval = await runInThread(lambda: socket.getsockopt(_level, _option))

        except Exception as ex:
            response.err = logAndConvertException(
                f'socketOptionGet failed handle={request.handle}', ex)

        return responseMsg

    async def _handleSocketListen(self, request: networking.MsgSocketListenRequest) -> networking.MsgResponse:
        debug(
            f'socketOptionListen handle={request.handle} backlock={request.backlog} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketListenResponse

        try:
            socketCtx = self._getSocketCtx(request.handle)

            socketCtx.setTimeoutMsec(request.timeout)
            await runInThread(lambda: socketCtx.socket.listen(request.backlog))

            response.err = 0

        except Exception as ex:
            response.err = logAndConvertException(
                f'socketListen failed handle={request.handle}', ex)

        return responseMsg

    async def _handleSocketAccept(self, request: networking.MsgSocketAcceptRequest) -> networking.MsgResponse:
        debug(
            f'socketAccept handle={request.handle} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketAcceptResponse

        response.handle = -1
        response.address.port = 0
        response.address.ip = 0

        try:
            handle = self._getFreeHandle()
            socketCtx = self._getSocketCtx(request.handle)

            socketCtx.setTimeoutMsec(request.timeout)

            (connection, address) = await runInThread(lambda: socketCtx.socket.accept())
            self._sockets[handle] = SocketContext(connection, socketCtx.type)

            response.handle = handle
            serializeAddress(address, response.address)
            response.err = 0

        except Exception as ex:
            response.err = logAndConvertException(
                f'socketAccept failed handle={request.handle}', ex)

        return responseMsg

    async def _handleSocketShutdown(self, request: networking.MsgSocketShutdownRequest) -> networking.MsgResponse:
        debug(
            f'socketAccept handle={request.handle} timeout={request.timeout}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketShutdownResponse

        response.err = 0

        try:
            socketCtx = self._getSocketCtx(request.handle)
            socket = socketCtx.socket

            socketCtx.setTimeoutMsec(request.timeout)

            await runInThread(lambda: socket.shutdown(shutdownHow(request.direction)))

        except Exception as ex:
            response.err = logAndConvertException(
                f'socketShutdown failed handle={request.handle}', ex)

        return responseMsg

    async def _closeAllSockets(self):
        async def closeCtx(ctx):
            try:
                ctx.setTimeoutMsec(None)

                await ctx.close()
            except Exception as ex:
                warning(
                    f'failed to close socket: {formatException(ex)}')

        contexts = [ctx for ctx in self._sockets if ctx != None]

        if len(contexts) > 0:
            await asyncio.wait([asyncio.create_task(closeCtx(ctx)) for ctx in contexts])

    def _getSocketCtx(self, handle: int) -> SocketContext:
        if handle < 1 or handle > MAX_HANDLE or self._sockets[handle] is None:
            raise InvalidHandleError(handle)

        socketCtx = self._sockets[handle]
        assert socketCtx is not None

        return socketCtx

    def _getFreeHandle(self) -> int:
        for handle, ctx in enumerate(self._sockets):
            if handle > 0 and ctx == None:
                return handle

        raise NoMoreSocketsError()

    def _fdSetToHandles(self, fds: int, width: int) -> List[int]:
        width = max(min(width, 32), 0)

        return [handle for handle in range(1, width) if (fds & (1 << handle) > 0)]

    def _fdSetToSockets(self, fds: int, width: int) -> List[socket.socket]:
        return [cast(SocketContext, self._sockets[handle]).socket for handle in self._fdSetToHandles(fds, width) if self._sockets[handle] is not None]

    def _socketsToFdSet(self, sockets):
        packed = 0

        for handle, ctx in enumerate(self._sockets):
            if ctx == None or not ctx.socket in sockets:
                continue

            packed |= 1 << handle

        return packed
