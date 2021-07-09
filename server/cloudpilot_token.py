import base64
import binascii
import hashlib
import math
from secrets import token_bytes
from time import time

SECRET = token_bytes(16)
TOKEN_TTL = 10

def generateToken():
    timestamp = math.floor(time())
    timeLE = bytes((timestamp & 0xff, (timestamp >> 8) & 0xff, (timestamp >> 16) & 0xff, (timestamp >> 24) & 0xff))

    nonce = token_bytes(16)

    hash = hashlib.new('sha256')
    hash.update(nonce + timeLE + SECRET)

    return base64.b64encode(nonce + timeLE + hash.digest()).decode('ascii')

def validateToken(token: str):
    if len(token) != 72:
        return False

    decodedToken = None
    try:
        decodedToken = base64.b64decode(token)
    except binascii.Error:
        return False

    timestampToken = decodedToken[16] | (decodedToken[17] << 8) | (decodedToken[18] << 16) | (decodedToken[19] << 24)
    timestamp = math.floor(time())

    if timestamp < timestampToken or (timestamp - timestampToken) > TOKEN_TTL:
        return False

    hash = hashlib.new('sha256')
    hash.update(decodedToken[0:20] + SECRET)

    return hash.digest() == decodedToken[20:]
