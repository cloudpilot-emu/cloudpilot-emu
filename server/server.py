
from asyncio.exceptions import CancelledError

from sanic import Sanic

from connection import Connection
from logger import logger


def start(host, port, ssl, loglevel):
    app = Sanic(name="Cloudpilot server")
    connections = set()

    logger.setLevel(loglevel.upper())

    @app.websocket("/")
    async def handleProxyConnection(request, socket):
        connections.add(socket)

        connection = Connection()

        await connection.start(socket)

        connections.remove(socket)

    app.run(host=host, port=port, ssl=ssl)
