#!/usr/bin/env python3

import argparse
import ssl
import sys

import server
from certificate import generateCertificate
from version import VERSION


def launchServer(options):
    sslCtx = None
    if options.cert != None:
        try:
            sslCtx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
            sslCtx.load_cert_chain(options.cert)
            sslCtx.get_ca_certs()
        except Exception as ex:
            print(f'unable to load certificate: ${ex}')
            exit(1)

    if sslCtx == None and not options.insecure:
        print("""
No certificate specified. Please generate a certificate by running

 > {script} generate-cert

and then run the server with the generated certificate via

 > {script} serve --cert <generate-certificate.pem>

Please follow the documentation at

https://github.com/cloudpilot-emu/cloudpilot/blob/master/doc/networking.md

in order to install the certificate on your device and set up networking in
Cloudpilot.
""".format(script=sys.argv[0]))
        return

    if sslCtx == None:
        print("""
WARNING: server running without SSL. This will not work with the official SPA at
https://cloudpilot-emu.github.org (or any other instance of Cloudpilot served
over HTTPS). Generate a certificate and specifiy it via --cert in order to run
with SSL enabled. Please check the documentation for more details.
""")

    if sslCtx == None and options.basicAuth != None:
        print("""WARNING: Basic auth enabled without SSL. This is INSECURE.
""")

    defaultPort = 8666 if sslCtx == None else 8667
    port = defaultPort if options.port == None else options.port

    server.start(host=options.host, port=port,
                 ssl=sslCtx, logLevel=options.log, logLevelFramework=options.logLevelFramework,
                 trustedOrigins=options.trustedOrigins, forceBindAddress=options.forceBind, authentication=options.basicAuth)


parser = argparse.ArgumentParser(description="Proxy server for Cloudpilot")

subparsers = parser.add_subparsers(help="subcommand", dest="subcommand")

parserServe = subparsers.add_parser(
    "serve", help="start server", description="Start proxy server")

parserServe.add_argument('--port', '-p', default=None,
                         type=int, help="server port [default: 8667 (SSL) / 8666 (insecure)]")

parserServe.add_argument('--bind', '-b', default="0.0.0.0",
                         help="bind server to host [default: 0.0.0.0]", dest="host")

parserServe.add_argument("--log", help="log level [default: info]", default="info", dest="log",
                         choices=["error", "warning", "info", "debug"])

parserServe.add_argument("--framework-log", help="log level for web framework [default: warrning]",
                         default="warning", dest="logLevelFramework", choices=["error", "warning", "info", "debug"])

parserServe.add_argument("--cert", help="enable TLS encryption using the specified certificate in PEM format",
                         default=None)

parserServe.add_argument(
    "--insecure", help="run unencrypted", default=False, action="store_true")

parserServe.add_argument("--trusted-origins", help="trusted origins for CORS [default: https://cloudpilot-emu.github.io]",
                         default="https://cloudpilot-emu.github.io", dest="trustedOrigins")

parserServe.add_argument("--force-bind", help="force bind all outgoing connections to the specified address",
                         default=None, dest="forceBind")

parserServe.add_argument("--basic-auth", help="enable basic auth against the provided username:password",
                         default=None, dest="basicAuth")


parserGenerateCert = subparsers.add_parser("generate-cert", help="generate certificate",
                                           description="Generate self-signed certificate and exit")

parserGenerateCert.add_argument("--overwrite", help="overwrite certificate files",
                                default=False, action="store_true", dest="overwrite")

parserGenerateCert.add_argument(
    "--name", help="certificate name", default=None, dest="name")

parserGenerateCert.add_argument("--names", help="certificate names (IPs, hostnames, domains)",
                                default=None, dest="names")

parserVersion = subparsers.add_parser(
    "version", help="print server version", description="Print server version and exit.")

options = parser.parse_args()

if options.subcommand == "serve":
    launchServer(options)

elif options.subcommand == "generate-cert":
    generateCertificate(options)

elif options.subcommand == "version":
    print(f"cloudpilot-server version {VERSION}")

else:
    parser.print_help()
