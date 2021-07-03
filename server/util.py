import asyncio


def runInThread(func):
    return asyncio.get_event_loop().run_in_executor(None, func)
