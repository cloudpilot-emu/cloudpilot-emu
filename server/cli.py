import argparse
import asyncio
import logging
import signal

from logger import logger
from server import Server

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

server = Server()
eventLoop = asyncio.get_event_loop()
task = eventLoop.create_task(server.start(options.host, options.port))
tasks = [task]

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
