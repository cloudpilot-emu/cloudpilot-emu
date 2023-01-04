export function memoize<T, U>(fn: (x: T) => U): (x: T) => U {
    let memoizedInput: T | undefined = undefined;
    let memoizedResult: U | undefined;
    let hasMemoizedInput = false;

    return (x: T): U => {
        if (!hasMemoizedInput || x !== memoizedInput) {
            memoizedInput = x;
            hasMemoizedInput = true;
            memoizedResult = fn(x);
        }

        return memoizedResult!;
    };
}
