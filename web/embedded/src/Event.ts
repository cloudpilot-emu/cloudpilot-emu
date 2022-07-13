export interface Event<Payload> {
    addHandler<Context>(handler: Handler<Payload, Context>, context?: Context): Event<Payload>;
    removeHandler<Context>(handler: Handler<Payload, Context>, context?: Context): Event<Payload>;
}

export type Handler<Payload, Context> = (payload: Payload, context: Context) => void;
