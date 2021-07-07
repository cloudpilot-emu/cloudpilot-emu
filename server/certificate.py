import os
import random
import re
import readline
import sys

from OpenSSL import crypto

CN_DEFAULT = "cloudpilot-server"
VALIDITY_YEARS = 1
BASIC_CONSTRAINTS = b'CA:TRUE,pathlen:0'
KEY_USAGE = b'serverAuth'
EXTENDED_KEY_USAGE = b'keyEncipherment,keyAgreement,cRLSign,digitalSignature'

REGEX_IP = re.compile('^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$')
REGEX_NAME = re.compile('^[a-zA-Z\d\.\-]+$')

def _deleteIfRequired(file, overwrite):
    if not os.path.exists(file):
        return

    if os.path.isdir(file):
        print(f'{file} exists and is a directory, aborting...')
        exit(1)

    if not overwrite:
        print(f'{file} exists, aborting... specifiy "--overwrite" if you want to overwrite it instead')
        exit(1)

    try:
        os.remove(file)
    except Exception as ex:
        print(f'ERROR: unable to delete {file}: {ex}')
        exit(1)

def _decomposeNames(namestring):
    parts = [name.strip() for name in namestring.split(",")]
    ips = []
    names = []

    for part in parts:
        if REGEX_IP.match(part):
            ips.append(part)

        elif REGEX_NAME.match(part):
            names.append(part)

        else:
            print(f'ERROR: {part} is neither a valid IP, name or domain')
            return (None, None)

    return (ips, names)

def _inputNames():
    print('please enter a comma separated list of IPs, hostnames or domains for which this cert will be valid:')
    return _decomposeNames(input())

def generateCertificate(options):
    cn = None

    if options.name == None:
        cn = input(f'certificate name (enter for {CN_DEFAULT}): ')
        cn = cn if cn else CN_DEFAULT
        print()
    else:
        cn = options.name

    filePem = cn + ".pem"
    fileCer = cn + ".cer"

    _deleteIfRequired(filePem, options.overwrite)
    _deleteIfRequired(fileCer, options.overwrite)

    ips = None
    names = None

    if options.names == None:
        while ips == None or names == None:
            ips, names = _inputNames()
            print()
    else:
        ips, names = _decomposeNames(options.names)

        if ips == None or names == None:
            exit(1)

    print("generating key and certificate...")

    key = crypto.PKey()
    key.generate_key(crypto.TYPE_RSA, 4096)

    cert = crypto.X509()
    cert.set_version(2)
    cert.get_subject().CN = cn

    basicConstraints = crypto.X509Extension(b"basicConstraints", True, BASIC_CONSTRAINTS)
    subjectAltName = crypto.X509Extension(b"subjectAltName", False,
        bytes(",".join([f'IP:{ip}' for ip in ips] + [f'DNS:{name}' for name in names]), "utf8")
    )
    extendedKeyUsage = crypto.X509Extension(b'extendedKeyUsage', True, KEY_USAGE)
    keyUsage = crypto.X509Extension(b'keyUsage', True, EXTENDED_KEY_USAGE)

    cert.add_extensions((basicConstraints, keyUsage, extendedKeyUsage, subjectAltName))

    random.seed()
    cert.set_serial_number(random.randint(0, 0xffff))

    cert.gmtime_adj_notBefore(0)
    cert.gmtime_adj_notAfter(VALIDITY_YEARS * 365 * 24 * 3600)

    cert.set_issuer(cert.get_subject())
    cert.set_pubkey(key)
    cert.sign(key, "sha256")

    try:
        with open(filePem, "wb") as file:
            file.write(crypto.dump_certificate(crypto.FILETYPE_PEM, cert))
            file.write(crypto.dump_privatekey(crypto.FILETYPE_PEM, key))
    except Exception as ex:
        print(f'failed to write {filePem}: {ex}')
        exit(1)

    try:
        with open(fileCer, "wb") as file:
            file.write(crypto.dump_certificate(crypto.FILETYPE_ASN1, cert))
    except Exception as ex:
        print(f'failed to write {fileCer}: {ex}')
        exit(1)

    print("""...done! You can now launch the server with

> {script} --cert {filePem}

in order to use the generated certificate. Please check the documentation on
how to install and trust "{fileCer}" on your devices running Cloudpilot.

SECURITY WARNING:
===============================================================================
An attacker that gets hold of "{filePem}" could abuse it and attempt
to mount a man-in-the-middle attack on SSL connections that originate
from devices that have "{fileCer}" installed and trusted.

"{filePem}" is sensitive information; DO NOT DISTRIBUTE IT.
===============================================================================
""".format(script=sys.argv[0], filePem=filePem, fileCer=fileCer))
