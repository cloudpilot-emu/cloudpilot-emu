import asyncio
import websockets
import proto.networking_pb2 as networking


class ProxyContext:
    async def start(self, socket):
        self._socket = socket

        try:
            while True:
                message = await socket.recv()

                print("\nincoming request")

                await self._handleMessage(message)

        except websockets.exceptions.ConnectionClosedError:
            print("connection closed")

    async def _handleMessage(self, message):
        reqest = networking.MsgRequest()
        reqest.ParseFromString(message)

        print(reqest)

        requestType = reqest.WhichOneof("payload")

        if (requestType == "socketOpenRequest"):
            await self._handleSocketOpen(reqest.socketOpenRequest)

        else:
            print(f'unknown request {requestType}')

    async def _handleSocketOpen(self, request):
        print(
            f'socketOpenRequest: domain={request.domain} type={request.type} protocol={request.protocol}')

        response = networking.MsgResponse()
        response.socketOpenResponse.handle = 42

        await self._socket.send(response.SerializeToString())


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
