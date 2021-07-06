#!/usr/bin/env python3

import argparse
import asyncio
import logging
import platform
import signal
import ssl

from certificate import generateCertificate
from logger import logger
from server import Server


def launchServer(options):
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

    else:
        print("""
WARNING: server running without SSL. This will not work with the official SPA at
https://cloudpilot-emu.github.org (or any other instance of Cloudpilot server
over HTTPS). Generate a certificate and specifiy it via --cert in order to run
with SSL enabled. Please check the documentation for more details.
""")

    server = Server()
    eventLoop = asyncio.get_event_loop()

    defaultPort = 8666 if sslCtx == None else 8667
    port = defaultPort if options.port == None else options.port

    task = eventLoop.create_task(server.start(
        options.host, port, sslCtx))
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


parser = argparse.ArgumentParser(description="Proxy server for Cloudpilot")

subparsers = parser.add_subparsers(help="subcommand", dest="subcommand")

parserServe = subparsers.add_parser("serve", help="start server", description="Start proxy server")

parserServe.add_argument('--port', '-p', default=None,
                    type=int, help="server port [default: 8666 / 8667 (TLS)]")

parserServe.add_argument('--bind', '-b', default="0.0.0.0",
                    help="bind server to host [default: 0.0.0.0]", dest="host")

parserServe.add_argument("--log", help="log level [default: info]", default="info", dest="log",
                    choices=["error", "warning", "info", "debug"])

parserServe.add_argument("--cert", help="enable TLS encryption using the specified certificate in PEM format",
    default=None)

parserGenerateCert = subparsers.add_parser("generate-cert", help="generate certificate",
    description="Generate self-signed certificate and exit")

parserGenerateCert.add_argument("--overwrite", help="overwrite certificate files",
    default=False, action="store_true", dest="overwrite")

parserGenerateCert.add_argument("--name", help="certificate name", default=None, dest="name")

parserGenerateCert.add_argument("--names", help="certificate names (IPs, hostnames, domains)",
    default=None, dest="names")

options = parser.parse_args()

if options.subcommand == "serve":
    launchServer(options)

elif options.subcommand == "generate-cert":
    generateCertificate(options)

else:
    parser.print_help()
