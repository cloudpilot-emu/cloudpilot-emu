import asyncio
import websockets
import proto.networking_pb2 as networking
import hexdump


class ProxyContext:
    def __init__(self):
        self.echoRequest = None

    async def start(self, socket):
        self._socket = socket

        try:
            while True:
                message = await socket.recv()

                await self._handleMessage(message)

        except websockets.exceptions.ConnectionClosedError:
            print("connection closed")

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
            print(f'unknown request {requestType}')

        if response:
            response.id = request.id

            await self._socket.send(response.SerializeToString())

    async def _handleSocketOpen(self, request):
        print(
            f'socketOpenRequest: type={request.type} protocol={request.protocol}')

        response = networking.MsgResponse()
        response.socketOpenResponse.handle = 42
        response.socketOpenResponse.err = 0

        return response

    async def _handleSocketBind(self, request):
        print(
            f'socketBindRequest: handle={request.handle} ip={request.address.ip} port={request.address.port}')

        response = networking.MsgResponse()
        response.socketBindResponse.err = 0

        return response

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

        response = networking.MsgResponse()
        response.socketCloseResponse.err = 0

        return response


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
