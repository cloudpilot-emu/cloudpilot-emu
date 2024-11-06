import errno
import socket as sock
import time
from typing import Optional

from util import runInThread

MAX_TIMEOUT = 10


class SocketContext:
    socket: sock.socket
    type: sock.SocketKind
    bound: bool
    nonblocking: bool
    _timeout: Optional[float]
    _timeoutBase: Optional[float]

    def __init__(self, socket: sock.socket, type: sock.SocketKind):
        self.socket = socket
        self.type = type
        self.bound = False
        self.nonblocking = False

        self._timeout = None
        self._timoutBase = time.time()

    def setTimeoutMsec(self, timeout: float):
        self._timeout = min(
            MAX_TIMEOUT, timeout / 1000) if timeout != None and timeout >= 0 else None
        self._timeoutBase = time.time()

        self.updateTimeout()

    def updateTimeout(self):
        if self.nonblocking:
            self.socket.settimeout(0)
            return

        if self._timeout == None:
            self.socket.settimeout(MAX_TIMEOUT)

        else:
            self.socket.settimeout(
                max(0, self._timeout - time.time() + self._timeoutBase))

    def setNonblocking(self, value: bool):
        self.nonblocking = value
        self.updateTimeout()

    def getNonblocking(self) -> bool:
        return self.nonblocking

    async def close(self):
        self.updateTimeout()

        try:
            await runInThread(lambda: self.socket.shutdown(sock.SHUT_RDWR))
        except OSError as ex:
            if ex.errno != errno.ENOTCONN:
                raise ex

        self.updateTimeout()
        await runInThread(lambda: self.socket.close())
