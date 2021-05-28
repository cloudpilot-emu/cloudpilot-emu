import asyncio
import websockets
import message_pb2


async def handle(socket, path):
    print("connection to %s" % path)

    await socket.send("hulpewulpe")

    try:
        while True:
            message = await socket.recv()
            deserializedMessage = message_pb2.ContainerMessage()

            deserializedMessage.ParseFromString(message)
            print(deserializedMessage.IsInitialized())

            print(deserializedMessage)

            await socket.send("holpewolpe")
    except websockets.exceptions.ConnectionClosedError:
        print("connection closed")
        pass

server = websockets.serve(handle, port=6666)
asyncio.get_event_loop().run_until_complete(server)
asyncio.get_event_loop().run_forever()
