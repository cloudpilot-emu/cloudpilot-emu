from OpenSSL import crypto


def generateCertificate():
    print("generating key...")

    key = crypto.PKey()
    key.generate_key(crypto.TYPE_RSA, 4096)

    req = crypto.X509Req()
    req.get_subject().CN = "cloudpilot-server"

    req.set_pubkey(key)
    req.sign(key, "sha256")

    cert = crypto.X509()

    cert.get_subject().CN = "cloudpilot-server"

    basicConstraints = crypto.X509Extension(b"basicConstraints", True, b"CA:TRUE")
    subjectAltName = crypto.X509Extension(b"subjectAltName", False, b"IP:192.168.178.20,IP:192.168.178.89")
    cert.add_extensions((basicConstraints, subjectAltName))

    cert.set_serial_number(666)
    cert.gmtime_adj_notBefore(0)
    cert.gmtime_adj_notAfter(315360000)

    cert.set_issuer(cert.get_subject())
    cert.set_pubkey(key)
    cert.sign(key, "sha256")

    with open("test.pem", "wb") as file:
        file.write(crypto.dump_certificate(crypto.FILETYPE_PEM, cert))
        file.write(crypto.dump_privatekey(crypto.FILETYPE_PEM, key))

    with open("test.cer", "wb") as file:
        file.write(crypto.dump_certificate(crypto.FILETYPE_ASN1, cert))
