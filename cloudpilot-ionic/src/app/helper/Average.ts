export class Average {
    constructor(capacity: number, initialValue?: number) {
        this.queue = new Array(capacity).fill(0);

        this.reset(initialValue);
    }

    reset(initialValue = 0) {
        this.next = 1;
        this.size = 1;

        this.queue[0] = initialValue;
    }

    push(value: number): this {
        this.queue[this.next] = value;
        this.next = (this.next + 1) % this.queue.length;

        if (this.size < this.queue.length) this.size++;

        return this;
    }

    calculateAverage(): number {
        let acc = 0;

        for (let i = 0; i < this.size; i++) acc += this.queue[i];

        return acc / this.size;
    }

    private queue: Array<number>;
    private next!: number;
    private size!: number;
}
