export class Fifo<T> {
    constructor(size: number) {
        this.buffer = new Array(size);
    }

    push(value: T): this {
        if (this.size < this.buffer.length) {
            this.buffer[(this.start + this.size++) % this.buffer.length] = value;
        } else {
            this.start = (this.start + 1) % this.buffer.length;

            this.buffer[(this.start + this.size - 1) % this.buffer.length] = value;
        }

        return this;
    }

    pop(): T | undefined {
        if (this.size === 0) return undefined;

        const elt = this.buffer[this.start];

        this.start = (this.start + 1) % this.buffer.length;
        this.size--;

        return elt;
    }

    peekLast(): T | undefined {
        if (this.size === 0) return undefined;

        return this.buffer[(this.start + this.size - 1) % this.buffer.length];
    }

    replaceLast(value: T): this {
        if (this.size > 0) {
            this.buffer[(this.start + this.size - 1) % this.buffer.length] = value;
        }

        return this;
    }

    count(): number {
        return this.size;
    }

    flush(): this {
        this.start = 0;
        this.size = 0;

        return this;
    }

    private buffer: Array<T>;

    private start = 0;
    private size = 0;
}
