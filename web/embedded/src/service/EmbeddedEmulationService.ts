import { Cloudpilot, SessionImage } from '@common/Cloudpilot';

import { AbstractEmulationService } from '@common/service/AbstractEmulationService';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { Event } from 'microevent.ts';
import { SchedulerKind } from '@pwa/helper/scheduler';
import { Session } from '@embedded/model/Session';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { hostname } from 'os';

export class EmbeddedEmulationService extends AbstractEmulationService {
    pause(): void {
        this.doPause();
    }

    resume(): void {
        this.doResume();
    }

    initWithRom(cloudpilot: Cloudpilot, rom: Uint8Array, device: DeviceId, session: Session): boolean {
        if (this.isRunning()) {
            this.doStop();
            this.session = undefined;
        }

        if (this.openSession(cloudpilot, rom, device)) {
            this.session = session;
            return true;
        }

        return false;
    }

    initWithSessionImage(cloudpilot: Cloudpilot, sessionImage: SessionImage<unknown>, session: Session): boolean {
        if (this.isRunning()) {
            this.doStop();
            this.session = undefined;
        }

        if (
            this.openSession(
                cloudpilot,
                sessionImage.rom,
                sessionImage.deviceId,
                sessionImage.memory,
                sessionImage.savestate
            )
        ) {
            this.session = session;

            return true;
        }

        return false;
    }

    getSession(): Session | undefined {
        return this.session;
    }

    forceUpdateHotsyncName(): void {
        if (!this.cloudpilotInstance) return;

        if (this.cloudpilotInstance.isUiInitialized() && !this.cloudpilotInstance.isPowerOff()) {
            this.checkAndUpdateHotsyncName();
        }
    }

    protected getConfiguredSpeed(): number {
        return this.session?.speed ?? 1;
    }

    protected manageHotsyncName(): boolean {
        return true;
    }

    protected getConfiguredHotsyncName(): string | undefined {
        return this.session?.hotsyncName;
    }

    protected updateConfiguredHotsyncName(hotsyncName: string): void {
        if (this.session) {
            this.session.hotsyncName = hotsyncName;
        }
    }

    protected getConfiguredSchdedulerKind(): SchedulerKind {
        return this.session?.runInBackground ? SchedulerKind.timeout : SchedulerKind.animationFrame;
    }

    protected hasFatalError(): boolean {
        return !!this.cloudpilotInstance?.hasFatalError();
    }

    protected skipEmulationUpdate(): boolean {
        return false;
    }

    protected override onAfterAdvanceEmulation(timestamp: number, cycles: number): void {
        this.timesliceEvent.dispatch();
    }

    protected override handleInput(): boolean {
        return this.isRunning();
    }

    private session: Session | undefined;

    readonly timesliceEvent = new Event<void>();
}
