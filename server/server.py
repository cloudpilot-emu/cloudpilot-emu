import asyncio
import websockets
import proto.networking_pb2 as proto


async def handle(socket, path):
    print("connection to %s" % path)

    try:
        while True:
            message = await socket.recv()

            msgRequest = proto.MsgRequest()
            msgRequest.ParseFromString(message)

            print(msgRequest)

            requestType = msgRequest.WhichOneof("payload")

            if requestType == "socketOpenRequest":
                socketOpenRequest = msgRequest.socketOpenRequest

                print(
                    f'socketOpenRequest: domain={socketOpenRequest.domain} type={socketOpenRequest.type} protocol={socketOpenRequest.protocol}')

                msgResponse = proto.MsgResponse()
                msgResponse.socketOpenResponse.handle = 42

                await socket.send(msgResponse.SerializeToString())

            else:
                print(f'unknown message {requestType}')

    except websockets.exceptions.ConnectionClosedError:
        print("connection closed")
        pass

server = websockets.serve(handle, port=6666)
asyncio.get_event_loop().run_until_complete(server)
asyncio.get_event_loop().run_forever()
