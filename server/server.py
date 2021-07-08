
from asyncio.exceptions import CancelledError

import sanic.response as response
from sanic import Sanic
from sanic.exceptions import InvalidUsage, NotFound

from connection import Connection
from logger import logger


def start(host, port, ssl, loglevel):
    app = Sanic(name="Cloudpilot server")
    app.config.GRACEFUL_SHUTDOWN_TIMEOUT = 1

    logger.setLevel(loglevel.upper())

    @app.websocket("/")
    async def handleProxyConnection(request, socket):
        connection = Connection()

        await connection.handle(socket)

    @app.exception(NotFound)
    async def handle404(request, exception):
        return response.text("not found", 404)

    @app.exception(InvalidUsage)
    async def handle400(request, exception):
        return response.text("bad request", 400)

    app.run(host=host, port=port, ssl=ssl)
