/**
 * A CloudpilotEmu event.
 */
export interface Event<Payload> {
    /**
     * Bind an handler callback to the event.
     *
     * CAUTION: a handler that is bound multiple times will be called multiple times, too!
     *
     * @param handler Handler callback.
     * @param context Optional context that is passed to the callback.
     */
    addHandler<Context>(handler: Handler<Payload, Context>, context?: Context): Event<Payload>;

    /**
     * Remove a previously bound handler. Both callback and context must be identical for
     * the handler to be removed.
     *
     * CAUTION: this method will only remove one callback each call, even if a handler / callback
     * combo is bound multiple times.
     *
     * @param handler Handler callback.
     * @param context Optional callback context.
     */
    removeHandler<Context>(handler: Handler<Payload, Context>, context?: Context): Event<Payload>;
}

/**
 * Handler callback type.
 */
export type Handler<Payload, Context> = (payload: Payload, context: Context) => void;
