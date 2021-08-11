import asyncio
import errno
import socket
import time

from util import runInThread

MAX_TIMEOUT = 10


class SocketContext:
    def __init__(self, socket: socket.socket, type):
        self.socket = socket
        self.type = type
        self.bound = False

        self._timeout = None
        self._timoutBase = time.time()

    def setTimeoutMsec(self, timeout):
        self._timeout = min(
            MAX_TIMEOUT, timeout / 1000) if timeout != None and timeout > 0 else None
        self._timeoutBase = time.time()

        self.updateTimeout()

    def updateTimeout(self):
        if self._timeout == None and self.socket.getblocking():
            self.socket.settimeout(MAX_TIMEOUT)

        else:
            self.socket.settimeout(
                max(0, self._timeout - time.time() + self._timeoutBase))

    async def close(self):
        self.updateTimeout()

        try:
            await runInThread(lambda: self.socket.shutdown(socket.SHUT_RDWR))
        except OSError as ex:
            if ex.errno != errno.ENOTCONN:
                raise ex

        self.updateTimeout()
        await runInThread(lambda: self.socket.close())
