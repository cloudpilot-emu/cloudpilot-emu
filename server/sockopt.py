import socket
import struct

from palmos_constants import *

_lookup = {
    netSocketOptSockDebug: "SO_DEBUG",
    netSocketOptSockAcceptConn: "SO_ACCEPTCONN",
    netSocketOptSockReuseAddr: "SO_REUSEADDR",
    netSocketOptSockKeepAlive: "SO_KEEPALIVE",
    netSocketOptSockDontRoute: "SO_DONTROUTE",
    netSocketOptSockBroadcast: "SO_BROADCAST",
    netSocketOptSockUseLoopback: "SO_USELOOPBACK",
    netSocketOptSockLinger: "SO_LINGER",
    netSocketOptSockOOBInLine: "SO_OOBINLINE",
    netSocketOptSockSndBufSize: "SO_SNDBUF",
    netSocketOptSockRcvBufSize: "SO_RCVBUF",
    netSocketOptSockSndLowWater: "SO_SNDLOWAT",
    netSocketOptSockRcvLowWater: "SO_RCVLOWAT",
    netSocketOptSockSndTimeout: "SO_SNDTIMEO",
    netSocketOptSockRcvTimeout: "SO_RCVTIMEO",
    netSocketOptSockErrorStatus: "SO_ERROR",
    netSocketOptSockSocketType: "SO_TYPE",
}


def _lookupSockoptName(level, option):
    if level == netSocketOptLevelIP:
        if option == netSocketOptIPOptions:
            return "IP_OPTIONS"
        else:
            return None

    elif level == netSocketOptLevelTCP:
        if option == netSocketOptTCPNoDelay:
            return "TCP_NODELAY"
        elif option == netSocketOptTCPMaxSeg:
            return "TCP_MAXSEG"
        else:
            return None

    elif level == netSocketOptLevelSocket:
        return _lookup[option] if option in _lookup else None

    else:
        return None


def InvalidOptionError(Exception):
    pass


def translateSockoptOption(level, option):
    name = _lookupSockoptName(level, option)
    if name == None:
        raise InvalidOptionError()

    try:
        return getattr(socket, name)
    except AttributeError:
        return None


def translateSockoptLevel(level):
    if level == netSocketOptLevelIP:
        return socket.SOL_SOCKET

    if level == netSocketOptLevelIP:
        return socket.SOL_IP

    if level == netSocketOptLevelTCP:
        return socket.SOL_TCP

    return None


def sockoptValue(message):
    valueType = message.WhichOneof("value")

    if valueType == "boolval":
        return int(message.boolval)

    if valueType == "bufval":
        return message.bufval

    if message.level == netSocketOptLevelSocket and message.option == netSocketOptSockLinger:
        return struct.pack('ii', message.intval & 0xffff, (message.intval >> 16) & 0xffff)

    return message.intval
