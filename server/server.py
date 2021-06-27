
import asyncio

import websockets

from connection import Connection
from logger import logger


class Server:
    def __init__(self):
        self.connections = set()
        self.terminating = False
        self.server = None

    async def start(self, host, port):
        self.server = await websockets.serve(self._handle, host=host, port=port, close_timeout=1)
        print(f'server listening on {host}:{port}\n')

        await self.server.wait_closed()

        print("\nserver stopped")

    def stop(self):
        if self.server == None:
            return

        if not self.terminating:
            self.terminating = True

            self.server.close()

            for connection in self.connections:
                asyncio.create_task(connection.close())

        logger.info(
            f'waiting for {len(self.connections)} connections to close')

    async def _handle(self, socket, path):
        self.connections.add(socket)

        connection = Connection()
        await connection.start(socket)

        self.connections.remove(socket)
