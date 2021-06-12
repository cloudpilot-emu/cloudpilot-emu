import asyncio
from errno import errorcode
import errno
import websockets
import proto.networking_pb2 as networking
import hexdump
import socket
import net_errors as err

SOCKET_TYPE = {
    1: socket.SOCK_STREAM,
    2: socket.SOCK_DGRAM,
    3: socket.SOCK_RAW
}


class InvalidHandleError(Exception):
    def __init__(self, handle):
        super().__init__()
        self.handle = handle


def deserializeAddress(addr):
    return(
        f'{addr.ip & 0xff}.{(addr.ip >> 8) & 0xff}.{(addr.ip > 16) & 0xff}.{(addr .ip> 24) & 0xff}', addr.port)


def formatException(ex):
    if isinstance(ex, InvalidHandleError):
        return f'invalid handle {ex.handle}'

    return f'{type(ex).__name__}: {ex}'


def exceptionToErr(ex):
    if isinstance(ex, OSError):
        return err.errnoToPalm(ex.errno)

    if isinstance(ex, InvalidHandleError):
        return err.netErrParamErr

    return err.netErrInternal


async def closeSocket(sock):
    try:
        await asyncio.to_thread(lambda: sock.shutdown(socket.SHUT_RDWR))
    except OSError as ex:
        if ex.errno != errno.ENOTCONN:
            raise ex

    await asyncio.to_thread(lambda: sock.close())


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

        else:
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
            handle = self.nextHandle
            self.nextHandle += 1

            sockType = SOCKET_TYPE[request.type]
            sockProtocol = socket.getprotobyname(
                'icmp') if sockType == socket.SOCK_RAW else 0

            self._sockets[handle] = await asyncio.to_thread(lambda: socket.socket(
                socket.AF_INET, sockType, sockProtocol))

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
            await asyncio.to_thread(lambda: self._getSocket(request.handle).bind(
                deserializeAddress(request.address)))
            response.err = 0

        except Exception as ex:
            print(f'ERROR: failed to bind socket: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    async def _handleSocketAddr(self, request):
        print(f'socketAddrRequest: handle={request.handle}')

        response = networking.MsgResponse()
        response.socketAddrResponse.addressLocal.ip = 0
        response.socketAddrResponse.addressLocal.port = 0
        response.socketAddrResponse.addressRemote.ip = 0
        response.socketAddrResponse.addressRemote.port = 0
        response.socketAddrResponse.err = 0

        return response

    async def _handleSocketSend(self, request):
        print(
            f'socketSendRequest: handle={request.handle} len={len(request.data)} flags={request.flags} timeout={request.timeout} {f"ip={request.address.ip} port={request.address.port}" if request.HasField("address") else ""}\n')

        for line in hexdump.dumpgen(request.data):
            print(line)

        print()

        self.echoRequest = request.data

        response = networking.MsgResponse()
        response.socketSendResponse.err = 0
        response.socketSendResponse.bytesSent = len(request.data)

        return response

    async def _handeSocketReceive(self, request):
        print(
            f'socketReceiveRequest: handle={request.handle} flags={request.flags} timeout={request.timeout} maxLength{request.maxLen}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketReceiveResponse

        if self.echoRequest == None or len(self.echoRequest) < 24:
            response.err = 0x1200 | 17
            response.data = b''
            response.address.port = 0
            response.address.ip = 0
        else:
            response.err = 0

            echoResponse = bytearray(self.echoRequest)
            echoResponse[12:16] = self.echoRequest[16:20]
            echoResponse[16:20] = self.echoRequest[12:16]

            echoResponse[20] = 0x00

            checksum = echoResponse[22] | (echoResponse[23] << 8)
            checksum += 8
            echoResponse[22] = checksum & 0xff
            echoResponse[23] = (checksum >> 8) & 0xff

            self.echoRequest = None

            response.data = bytes(echoResponse)
            response.address.port = 0
            response.address.ip = 0

            print()
            for line in hexdump.dumpgen(response.data):
                print(line)
            print()

        return responseMsg

    async def _handleSocketClose(self, request):
        print(f'socketCloseRequest: handle={request.handle}')

        responseMsg = networking.MsgResponse()
        response = responseMsg.socketCloseResponse

        try:
            sock = self._getSocket(request.handle)
            del self._sockets[request.handle]

            await closeSocket(sock)
            response.err = 0

        except Exception as ex:
            print(
                f'ERROR: failed to close socket {request.handle}: {formatException(ex)}')
            response.err = exceptionToErr(ex)

        return responseMsg

    def _getSocket(self, handle):
        if not handle in self._sockets:
            raise InvalidHandleError(handle)

        return self._sockets[handle]

    async def _closeAllSockets(self):
        for handle, sock in self._sockets.items():
            try:
                await closeSocket(sock)
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
