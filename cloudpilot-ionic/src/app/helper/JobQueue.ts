export type Job = () => void | Promise<void>;

export class JobQueue {
    push(job: Job): void {
        this.state = this.state.then(async () => {
            try {
                await job();
            } catch (e) {}
        });
    }

    waitUntilEmpty(): Promise<void> {
        return this.state;
    }

    private state: Promise<void> = Promise.resolve();
}
