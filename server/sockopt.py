import socket
import struct
from typing import Optional, Union

from palmos_constants import *
from proto.networking_pb2 import MsgSocketOptionSetRequest

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


def _lookupSockoptName(level: int, option: int) -> Optional[str]:
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


def translateSockoptOption(level: int, option: int) -> Optional[int]:
    name = _lookupSockoptName(level, option)
    if name is None:
        return None

    try:
        return getattr(socket, name)
    except AttributeError:
        return None


def translateSockoptLevel(level: int) -> Optional[int]:
    if level == netSocketOptLevelSocket:
        return socket.SOL_SOCKET

    if level == netSocketOptLevelIP:
        return socket.SOL_IP

    if level == netSocketOptLevelTCP:
        return socket.SOL_TCP

    return None


def sockoptValue(message: MsgSocketOptionSetRequest) -> Union[bytes, int]:
    valueType = message.WhichOneof("value")

    if valueType == "bufval":
        return message.bufval

    if message.level == netSocketOptLevelSocket and message.option == netSocketOptSockLinger:
        return struct.pack('ii', message.intval & 0xffff, (message.intval >> 16) & 0xffff)

    return message.intval
