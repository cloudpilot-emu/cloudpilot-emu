import base64
import binascii
import hmac
import math
from secrets import token_bytes
from time import time

SECRET = token_bytes(16)
TOKEN_TTL = 10


def generateToken() -> str:
    timestamp = math.floor(time())
    timeLE = bytes((timestamp & 0xff, (timestamp >> 8) & 0xff,
                   (timestamp >> 16) & 0xff, (timestamp >> 24) & 0xff))

    nonce = token_bytes(16)

    return base64.b64encode(nonce + timeLE + hmac.digest(SECRET, nonce + timeLE, 'sha256')).decode('ascii')


def validateToken(token: str) -> bool:
    if len(token) != 72:
        return False

    decodedToken = None
    try:
        decodedToken = base64.b64decode(token, validate=True)
    except binascii.Error:
        return False

    timestampToken = decodedToken[16] | (decodedToken[17] << 8) | (
        decodedToken[18] << 16) | (decodedToken[19] << 24)
    timestamp = math.floor(time())

    if timestamp < timestampToken or (timestamp - timestampToken) > TOKEN_TTL:
        return False

    return hmac.digest(SECRET, decodedToken[0:20], 'sha256') == decodedToken[20:]
