import { Cloudpilot, SessionImage } from '@common/bridge/Cloudpilot';
import { Engine } from '@common/engine/Engine';
import { SnapshotContainer } from '@common/engine/Snapshot';
import { SchedulerKind } from '@common/helper/scheduler';
import { DeviceId } from '@common/model/DeviceId';
import { AbstractEmulationService, Executor } from '@common/service/AbstractEmulationService';
import { Session } from '@embedded/model/Session';
import { Mutex } from 'async-mutex';
import { Event } from 'microevent.ts';

export class EmbeddedEmulationService extends AbstractEmulationService {
    constructor(
        cloudpilot: Cloudpilot,
        private uarmModuleFactory: () => Promise<WebAssembly.Module>,
    ) {
        super(Promise.resolve(cloudpilot));

        this.syncSettings();
    }

    pause = (): Promise<void> => this.mutex.runExclusive(() => this.doStop());

    resume = (): Promise<void> => this.mutex.runExclusive(() => this.doResume());

    initWithRom = (
        rom: Uint8Array,
        nand: Uint8Array | undefined,
        device: DeviceId,
        session: Session,
    ): Promise<boolean> =>
        this.mutex.runExclusive(async () => {
            if (this.isRunning()) {
                await this.doStop();
                this.session = undefined;
            }

            if (await this.openSession(rom, device, nand)) {
                this.setSession(session);
                return true;
            }

            return false;
        });

    initWithSessionImage = (sessionImage: SessionImage<unknown>, session: Session): Promise<boolean> =>
        this.mutex.runExclusive(async () => {
            if (this.isRunning()) {
                await this.doStop();
                this.session = undefined;
            }

            if (
                await this.openSession(
                    sessionImage.rom,
                    sessionImage.deviceId,
                    undefined,
                    sessionImage.memory,
                    sessionImage.savestate,
                )
            ) {
                this.setSession(session);
                return true;
            }

            return false;
        });

    getSession(): Session | undefined {
        return this.session;
    }

    syncSettings(): void {
        this.updateEngineSettings({
            manageHotsyncName: true,
            hotsyncName: this.session?.hotsyncName,
            speed: this.session?.speed ?? 1,
            targetMips: 100 * (this.session?.speed ?? 1),
            schedulerKind: this.session?.runInBackground ? SchedulerKind.timeout : SchedulerKind.animationFrame,
            runInBackground: this.session?.runInBackground ?? false,
        });
    }

    getEngine(): Engine | undefined {
        return this.engine;
    }

    protected override updateConfiguredHotsyncName(hotsyncName: string): void {
        if (this.session) {
            this.session.hotsyncName = hotsyncName;
        }
    }

    // eslint-disable-next-line @typescript-eslint/no-unused-vars
    protected override onAfterAdvanceEmulation(sliceSizeSeconds: number): void {
        this.timesliceEvent.dispatch();
    }

    protected override handleSuspend(): void {}

    protected override handleSnapshot(snapshot: SnapshotContainer): void {
        snapshot.release(false, 0, 0);
    }

    protected override getUarmModule(): Promise<WebAssembly.Module> {
        return this.uarmModuleFactory();
    }

    protected override handleFatalInNativeCode(error: Error): void {
        console.error(error);
    }

    protected override handlePalmosStateChange(): void {}

    protected override clandestineExecute: Executor = (fn) => fn();

    private setSession(session: Session) {
        this.session = session;
        this.syncSettings();
    }

    private session: Session | undefined;
    private mutex = new Mutex();

    readonly timesliceEvent = new Event<void>();
}
