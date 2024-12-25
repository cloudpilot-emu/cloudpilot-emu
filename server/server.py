import base64
import logging
import platform
from asyncio.proactor_events import _ProactorBasePipeTransport
from functools import wraps
from os import stat
from ssl import SSLContext
from typing import Optional

import aiohttp_cors  # type: ignore
from aiohttp import log, web
from aiohttp.web_response import Response
from cloudpilot_token import TOKEN_TTL, generateToken, validateToken
from connection import Connection
from logger import logger

VERSION = 3
COMPAT_VERSION = 3

INDEX_HTML = """
<html>
<head>
    <title>CloudpilotEmu Proxy Server</title>
    <style>
        body {
            font-family: sans-serif;
            max-width: 40em;
            margin: auto;
            padding: 1em;
            text-align: center;
        }
    </style>
</head>
<body>
    <h1>CloudpilotEmu Proxy Server</h1>
    <p>
        Congratulations! You browser can access the proxy server. You can now proceed to set
        up networking in CloudpilotEmu.
    </p>
</body>
</html>
"""


def start(host: str, port: int, ssl: Optional[SSLContext], logLevel: str, logLevelFramework: str, trustedOrigins: str,
          forceBindAddress: Optional[str] = None, authentication: Optional[str] = None, nameserver: Optional[int] = None,
          heartbeat: Optional[int] = None):
    routes = web.RouteTableDef()

    if heartbeat is not None and heartbeat <= 0:
        heartbeat = None

    async def handshakeHandler(request: web.Request) -> Response:
        if not _validateAuth(request, authentication):
            return web.Response(status=401, text="401: unauthorized")

        logger.info(f"issued token, lifetime {TOKEN_TTL} seconds")
        return web.json_response({'version': VERSION, 'compatVersion': COMPAT_VERSION, 'token': generateToken()})

    @routes.get("/network-proxy/connect")
    async def connectHandler(request: web.Request):
        if not ("token" in request.query and validateToken(request.query["token"])):
            return web.Response(status=403, text="403: forbidden")

        ws = web.WebSocketResponse(heartbeat=heartbeat)
        await ws.prepare(request)

        connection = Connection(forceBindAddress, nameserver)
        await connection.handle(ws)

        return ws

    @routes.get("/")
    async def indexHandler(request: web.Request):
        return web.Response(text=INDEX_HTML, content_type="text/html")

    _setupLogging(logLevel, logLevelFramework)

    app = web.Application()
    cors = aiohttp_cors.setup(app)

    resourceHandshake = cors.add(
        app.router.add_resource("/network-proxy/handshake"))
    cors.add(resourceHandshake.add_route("POST", handshakeHandler), {
        origin.strip(): aiohttp_cors.ResourceOptions(
            allow_headers=("Authorization",)
        ) for origin in trustedOrigins.split(",")
    })

    app.add_routes(routes)

    web.run_app(app, host=host, port=port, ssl_context=ssl)


def _setupLogging(logLevel: str, logLevelFramework: str):
    logging.basicConfig(level=logging.ERROR)

    for aiohttpLogger in (log.access_logger, log.server_logger):
        aiohttpLogger.setLevel(logLevelFramework.upper())

    logger.setLevel(logLevel.upper())


def _validateAuth(request: web.Request, authentication=None) -> bool:
    if authentication == None:
        return True

    if not "authorization" in request.headers:
        return False

    try:
        authHeader = request.headers.get("authorization")
        if authHeader is None:
            return False

        pieces = authHeader.split(" ")
        if len(pieces) != 2:
            return False

        if pieces[0].upper() != "BASIC":
            return False

        return base64.b64decode(pieces[1], validate=True).decode('utf8').strip() == authentication
    except:
        return False

# Work around https://github.com/aio-libs/aiohttp/issues/4324


def silence_event_loop_closed(func):
    @wraps(func)
    def wrapper(self, *args, **kwargs):
        try:
            return func(self, *args, **kwargs)
        except RuntimeError as e:
            if str(e) != 'Event loop is closed':
                raise
    return wrapper


if platform.system() == 'Windows':
    _ProactorBasePipeTransport.__del__ = silence_event_loop_closed(  # type: ignore
        _ProactorBasePipeTransport.__del__)
