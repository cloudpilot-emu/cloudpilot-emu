import argparse
import asyncio
import logging
import signal

import websockets

from logger import logger
from proxy_context import ProxyContext

connections = set()


async def handle(socket, path):
    connections.add(socket)

    context = ProxyContext()
    await context.start(socket)

    connections.remove(socket)


async def startServer(host, port):
    server = await websockets.serve(handle, host=host, port=port, close_timeout=1)
    print(f'server listening on {host}:{port}\n')

    terminating = False

    def onSignal():
        nonlocal terminating

        if not terminating:
            server.close()

            for connection in connections:
                asyncio.create_task(connection.close())

            terminating = True

        logger.info(f'waiting for {len(connections)} connections to close')

    for sig in (signal.SIGINT, signal.SIGTERM):
        asyncio.get_event_loop().add_signal_handler(sig, onSignal)

    await server.wait_closed()

    print("\nserver stopped")


parser = argparse.ArgumentParser(description="Socket proxy for Cloudpilot")

parser.add_argument('--port', '-p', default="8666",
                    type=int, help="server port [default: 8666]")

parser.add_argument('--bind', '-b', default="0.0.0.0",
                    help="bind server to host [default: 0.0.0.0]", dest="host")

parser.add_argument("--log", help="log level [default: info]", default="info", dest="log",
                    choices=["error", "warning", "info", "debug"])

options = parser.parse_args()

logging.basicConfig(format="%(asctime)s %(levelname)s: %(message)s")
logger.setLevel(options.log.upper())

asyncio.run(startServer(options.host, options.port))
