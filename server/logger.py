import logging

logger = logging.Logger("cloudpilot-server")

handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter("%(name)s:%(levelname)s: %(asctime)s: %(message)s"))

logger.addHandler(handler)

debug = logger.debug
info = logger.info
warning = logger.warning
error = logger.error
