export function debounce(): MethodDecorator {
    return (target: unknown, propertyKey: string | symbol, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;
        let pending = false;

        desc.value = async function (this: unknown, ...args: Array<unknown>) {
            if (pending) return;

            pending = true;
            try {
                return await oldMethod.apply(this, args);
            } finally {
                pending = false;
            }
        };

        return desc;
    };
}
