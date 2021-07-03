import argparse
import asyncio
import logging
import platform
import signal
import ssl

from logger import logger
from server import Server

parser = argparse.ArgumentParser(description="Socket proxy for Cloudpilot")

parser.add_argument('--port', '-p', default="8666",
                    type=int, help="server port [default: 8666]")

parser.add_argument('--bind', '-b', default="0.0.0.0",
                    help="bind server to host [default: 0.0.0.0]", dest="host")

parser.add_argument("--log", help="log level [default: info]", default="info", dest="log",
                    choices=["error", "warning", "info", "debug"])

parser.add_argument("--cert", help="enable TLS encryption using the specified certificate",
                    default=None)

options = parser.parse_args()

logging.basicConfig(format="%(asctime)s %(levelname)s: %(message)s")
logger.setLevel(options.log.upper())

sslCtx = None
if options.cert != None:
    try:
        sslCtx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        sslCtx.load_cert_chain(options.cert)
    except Exception as ex:
        print(f'unable to load certificate: ${ex}')
        exit(1)

server = Server()
eventLoop = asyncio.get_event_loop()
task = eventLoop.create_task(server.start(
    options.host, options.port, sslCtx))
tasks = [task]

print(f'running on {platform.python_implementation()} {platform.python_version()}')

try:
    for sig in (signal.SIGINT, signal.SIGTERM):
        eventLoop.add_signal_handler(sig, server.stop)
except NotImplementedError:
    pass

while not task.done():
    try:
        eventLoop.run_until_complete(asyncio.gather(*tasks))
    except KeyboardInterrupt:
        async def stop():
            server.stop()

        tasks = [task for task in tasks if not task.done()]
        tasks.append(eventLoop.create_task(stop()))
