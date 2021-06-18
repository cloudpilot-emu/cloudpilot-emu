import asyncio
from errno import errorcode
import errno
import websockets
import proto.networking_pb2 as networking
import hexdump
import socket
import select
import net_errors as err
import traceback

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


class BadPacketException(Exception):
    pass


class SocketContext:
    def __init__(self, socket, type):
        self.socket = socket
        self.type = type


def deserializeAddress(addr):
    return(
        f'{(addr.ip >> 24) & 0xff}.{(addr.ip >> 16) & 0xff}.{(addr.ip >> 8) & 0xff}.{addr.ip & 0xff}', addr.port)


def serializeIp(addr):
    try:
        parts = [int(x) for x in addr.split(".")]
    except Exception:
        raise InvalidAddressError(addr)

    if len(parts) != 4:
        raise InvalidAddressError(addr)

    return ((parts[0] << 24) | (parts[1] << 16) | (
        parts[2] << 8) | parts[3]) & 0xffffffff


def serializeAddress(addr, target):

    if type(addr) != tuple or len(addr) != 2:
        raise InvalidAddressError(addr)

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

    return err.netErrInternal


async def closeSocket(sock):
    try:
        await asyncio.to_thread(lambda: sock.shutdown(socket.SHUT_RDWR))
    except OSError as ex:
        if ex.errno != errno.ENOTCONN:
            raise ex

    await asyncio.to_thread(lambda: sock.close())


def ipFromIpPacket(packet):
    if len(packet) < 20:
        raise BadPacketException()

    return f'{packet[16]}.{packet[17]}.{packet[18]}.{packet[19]}'


def removeIpHeader(packet, socketCtx):
    if socketCtx.type != socket.SOCK_RAW or len(packet) < 20 or (packet[0] >> 4) != 4 or (packet[0] & 0x0f) < 5 or packet[9] != 1:
        return packet

    hlen = packet[0] & 0x0f
    return packet[4*hlen:]


class ProxyContext:
    def __init__(self):
        self.echoRequest = None
        self._sockets = {}
        self.nextHandle = 0

    async def start(self, ws):
        self._ws = ws

        try:
            while True:
                await self._handleMessage(await ws.recv())

        except websockets.exceptions.ConnectionClosedError:
            print("connection closed")

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

        else:
            response = networking.MsgResponse()
            response.invalidRequestResponse.tag = True

            print(f'ERROR: unknown request {requestType}')

        if response:
            response.id = request.id

            await self._ws.send(response.SerializeToString())

    async def _handleSocketOpen(self, request):
        print(
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
                    print(
                        f'unsupported protocol for RAW socket: {request.protocol}')
                    response.err = err.netErrParamErr

                    return responseMsg

            handle = self.nextHandle
            self.nextHandle += 1

            sock = await asyncio.to_thread(lambda: socket.socket(
                socket.AF_INET, sockType, sockProtocol))

            self._sockets[handle] = SocketContext(sock, sockType)

            response.handle = handle
            response.err = 0

        except Exception as ex:
            print(f'ERROR: failed to open socket: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketBind(self, request):
        print(
            f'socketBindRequest: handle={request.handle} ip={request.address.ip} port={request.address.port}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketBindResponse

        try:
            sock = self._getSocketCtx(request.handle).socket

            await asyncio.to_thread(lambda: sock.bind(
                deserializeAddress(request.address)))
            response.err = 0

        except Exception as ex:
            print(f'ERROR: failed to bind socket: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketAddr(self, request):
        print(
            f'socketAddrRequest: handle={request.handle} requestAddressLocal={request.requestAddressLocal} requestAddressRemote={request.requestAddressRemote}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketAddrResponse

        try:
            sock = self._getSocketCtx(request.handle).socket

            if request.requestAddressLocal:
                serializeAddress(await asyncio.to_thread(lambda: sock.getsockname()), response.addressLocal)

            if request.requestAddressRemote:
                serializeAddress(await asyncio.to_thread(lambda: sock.getpeername()), response.addressRemote)

            response.err = 0

        except Exception as ex:
            print(
                f'ERROR: failed to get socket addresses: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketSend(self, request):
        print(
            f'socketSendRequest: handle={request.handle} len={len(request.data)} flags={request.flags} timeout={request.timeout} {f"ip={request.address.ip} port={request.address.port}" if request.HasField("address") else ""}\n')

        for line in hexdump.dumpgen(request.data):
            print(line)

        print()

        self.echoRequest = request.data

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketSendResponse

        try:
            socketCtx = self._getSocketCtx(request.handle)
            sock = socketCtx.socket
            flags = translateFlags(request.flags)

            if request.timeout > 0:
                rlist, wlist, xlist = await asyncio.to_thread(lambda: select.select((), (sock,), (), request.timeout / 1000))

                if len(wlist) == 0:
                    response.err = err.netErrTimeout
                    response.bytesSent = -1

                    return responseMsg

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

        except Exception as ex:
            print(f'ERROR: failed to send: {formatException(ex)}')

            response.err = exceptionToErr(ex)
            response.bytesSent = -1

        return responseMsg

    async def _handeSocketReceive(self, request):
        print(
            f'socketReceiveRequest: handle={request.handle} flags={request.flags} timeout={request.timeout} maxLength={request.maxLen}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketReceiveResponse

        response.data = b''
        response.address.ip = 0
        response.address.port = 0

        try:
            sock = self._getSocketCtx(request.handle).socket
            flags = translateFlags(request.flags)

            if request.timeout > 0:
                rlist, wlist, xlist = await asyncio.to_thread(lambda: select.select((sock,), (), (), request.timeout / 1000))

                if len(rlist) == 0:
                    response.err = err.netErrTimeout

                    return responseMsg

            data, address = await asyncio.to_thread(lambda: sock.recvfrom(request.maxLen, flags))

            serializeAddress(address, response.address)
            response.data = data
            response.err = 0

            print()
            for line in hexdump.dumpgen(response.data):
                print(line)
            print()

        except Exception as ex:
            print(f'ERROR: failed to receive: {formatException(ex)}')

            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketClose(self, request):
        print(f'socketCloseRequest: handle={request.handle}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketCloseResponse

        try:
            sock = self._getSocketCtx(request.handle).socket
            del self._sockets[request.handle]

            await closeSocket(sock)
            response.err = 0

        except Exception as ex:
            print(
                f'ERROR: failed to close socket {request.handle}: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleGetHostByName(self, request):
        print(f'getHostByNameRequest name{request.name}')

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
                serializeIp(x) for x in addresses][:3]

            response.err = 0

        except Exception as ex:
            print(
                f'failed to resolve host {request.name}: {formatException(ex)}')

            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleGetServByName(self, request):
        print(
            f'getServByNameRequest name={request.name} protocol={request.protocol}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.getServByNameResponse
        response.port = 0

        try:
            response.port = await asyncio.to_thread(lambda: socket.getservbyname(
                request.name, request.protocol))

            response.err = 0

        except Exception as ex:
            print(f'failed to resolve service: {formatException(ex)}')

            response.err = err.netErrUnknownService

        return responseMsg

    def _getSocketCtx(self, handle):
        if not handle in self._sockets:
            raise InvalidHandleError(handle)

        return self._sockets[handle]

    async def _closeAllSockets(self):
        for handle, sockCtx in self._sockets.items():
            try:
                await closeSocket(sockCtx.socket)
            except Exception as ex:
                print(
                    f'ERROR: failed to close socket {handle}: {formatException(ex)}')


async def handle(socket, path):
    print(f"incoming connection for {path}")

    context = ProxyContext()
    await context.start(socket)


async def main():
    server = await websockets.serve(handle, port=6666)
    print("server running on port 6666")

    await server.wait_closed()

    print("server closed")

asyncio.run(main())
