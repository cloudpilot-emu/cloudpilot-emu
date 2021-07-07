import sanic.log

logger = sanic.log.logger.getChild("cloudpilot")

debug = logger.debug
info = logger.info
warning = logger.warning
error = logger.error
