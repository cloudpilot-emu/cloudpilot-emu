export function debounce(): any {
    return (target: any, propertyKey: string, desc: PropertyDescriptor) => {
        const oldMethod = desc.value;
        let pending = false;

        desc.value = async function (this: any) {
            if (pending) return;

            pending = true;
            try {
                return await oldMethod.apply(this, arguments);
            } finally {
                pending = false;
            }
        };

        return desc;
    };
}
