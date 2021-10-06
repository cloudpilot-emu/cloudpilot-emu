import asyncio
from typing import Awaitable, Callable, TypeVar

T = TypeVar('T')


def runInThread(func: Callable[[], T]) -> Awaitable[T]:
    return asyncio.get_event_loop().run_in_executor(None, func)
