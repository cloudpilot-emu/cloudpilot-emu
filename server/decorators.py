from functools import wraps


def no_cache():
    def decorator(f):
        @wraps(f)
        async def decorated_function(request, *args, **kwargs):
            response = await f(request, *args, **kwargs)

            response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
            response.headers["Pragma"] = "no-cache"
            response.headers["Expires"] = 0

            return response

        return decorated_function

    return decorator
