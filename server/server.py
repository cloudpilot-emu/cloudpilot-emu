
import asyncio

import sanic.log
import sanic.response as response
from sanic import Sanic
from sanic.exceptions import InvalidUsage, NotFound
from sanic.request import Request
from sanic_cors import cross_origin

from cloudpilot_token import TOKEN_TTL, generateToken, validateToken
from connection import Connection
from decorators import no_cache
from logger import logger

VERSION = 1

ROUTE_PROXY_SOCKET_CONNECT = "ROUTE_PROXY_SOCKET_CONNECT"
APP_NAME = "CLOUDPILOT"


def start(host, port, ssl, logLevel, logLevelSanic, validOrigins):
    app = Sanic(name=APP_NAME)
    app.config.GRACEFUL_SHUTDOWN_TIMEOUT = 1

    logger.setLevel(logLevel.upper())
    sanic.log.logger.setLevel(logLevelSanic.upper())
    sanic.log.access_logger.setLevel(logLevelSanic.upper())
    sanic.log.error_logger.setLevel(logLevelSanic.upper())

    print(
        f'server listening on {host}:{port}{" using SSL" if ssl != None else ""}\n')

    @app.websocket("/")
    @app.websocket("/network-proxy/connect", name=ROUTE_PROXY_SOCKET_CONNECT)
    async def initiateProxyConnection(request, socket):
        connection = Connection()

        await connection.handle(socket)

    @app.on_request
    async def authenticateProxyConnection(request: Request):
        if not request.route or request.route.name != f'{APP_NAME}.{ROUTE_PROXY_SOCKET_CONNECT}':
            return

        token = request.args.get("token")
        if not (isinstance(token, str) and validateToken(token)):
            logger.info("rejected proxy connect due to bad token")

            return response.text("forbidden", 403)

    @app.route("/network-proxy/token", methods=["GET", "OPTIONS"])
    @no_cache()
    @cross_origin(app,
                  origins="*" if validOrigins.strip() == "*" else [
                      origin.strip() for origin in validOrigins.split(",")]
                  )
    async def getToken(request):
        logger.info(f"issued token, lifetime {TOKEN_TTL} seconds")
        return response.json({'version': 1, 'token': generateToken()})

    @app.exception(NotFound)
    async def handle404(request, exception):
        return response.text("not found", 404)

    @app.exception(InvalidUsage)
    async def handle400(request, exception):
        return response.text("bad request", 400)

    @app.exception(asyncio.CancelledError)
    async def handleCancelled(request, exception):
        return response.text("internal serverr error", 500)

    @app.listener("after_server_stop")
    async def afterServerStop(app, loop):
        await asyncio.gather(*[task for task in asyncio.all_tasks() if task != asyncio.current_task()])

    app.run(host=host, port=port, ssl=ssl)
