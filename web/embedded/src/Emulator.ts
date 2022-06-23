import * as skin from '@common/skin';

import { Cloudpilot, ZipfileWalkerState } from '@common/Cloudpilot';
import { Event, EventInterface } from 'microevent.ts';
import { GenericEventHandlingService, HasEvents } from '@common/service/GenericEventHandlingService';

import { Button } from './index';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { EmbeddedCanvasDisplayService } from './service/EmbeddedCanvasDisplayService';
import { EmbeddedEmulationService } from './service/EmbeddedEmulationService';
import { EmulationStatistics } from '@common/model/EmulationStatistics';
import { Session } from './model/Session';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { loadSkin } from '@common/skin/loader';

const DEFAULT_SESSION: Session = {
    hotsyncName: undefined,
    orientation: DeviceOrientation.protrait,
    runInBackground: false,
    speed: 1,
    deviceId: DeviceId.m515,
};

export interface EmulatorInterface {
    loadRom(rom: Uint8Array, deviceId: DeviceId): this;

    loadSession(session: Uint8Array): this;

    setCanvas(canvas: HTMLCanvasElement): this;

    bindInput(pointerTarget: HasEvents, keyboardTarget?: HasEvents): this;
    releaseInput(): this;

    installDatabase(file: Uint8Array): this;
    installAndLaunchDatabase(file: Uint8Array): this;
    installFromZipfile(file: Uint8Array): this;
    installFromZipfileAndLaunch(file: Uint8Array, launchFile?: string): this;

    launchByName(name: string): this;
    launchDatabase(database: Uint8Array): this;

    reset(): this;
    resetNoExtensions(): this;
    resetHard(): this;

    isRunning(): boolean;
    isPowerOff(): boolean;
    isUiInitialized(): boolean;

    mute(): this;
    unmute(): this;
    autounmute(): this;

    resume(): this;
    pause(): this;

    buttonDown(button: Button): this;
    buttonUp(button: Button): this;

    setSpeed(speed: number): this;
    getSpeed(): number;

    setVolume(volume: number): this;
    getVolume(): number;

    setOrientation(orientation: DeviceOrientation): this;
    getOrientation(): DeviceOrientation;

    setHotsyncName(hotsyncName: string | undefined): this;
    getHotsyncName(): string | undefined;

    setRunHidden(toggle: boolean): this;
    getRunHidden(): boolean;

    getStatistics(): EmulationStatistics;

    readonly powerOffChangeEvent: EventInterface<boolean>;
    readonly isUiInitializedChangeEvent: EventInterface<boolean>;
    readonly audioInitializedEvent: EventInterface<void>;
    readonly timesliceEvent: EventInterface<void>;
}

export class Emulator implements EmulatorInterface {
    constructor(private cloudpilot: Cloudpilot) {
        this.emulationService = new EmbeddedEmulationService();
        this.canvasDisplayService = new EmbeddedCanvasDisplayService(loadSkin(Promise.resolve(skin)));
        this.eventHandlingService = new GenericEventHandlingService(this.emulationService, this.canvasDisplayService);

        this.emulationService.newFrameEvent.addHandler((canvas) =>
            this.canvasDisplayService.updateEmulationCanvas(canvas)
        );

        this.timesliceEvent = this.emulationService.timesliceEvent;
        this.timesliceEvent.addHandler(this.onTimeslice);

        this.powerOffCached = this.isPowerOff();
    }

    getStatistics(): EmulationStatistics {
        throw new Error('Method not implemented.');
    }

    loadRom(rom: Uint8Array, deviceId?: DeviceId): this {
        if (deviceId === undefined) {
            const rominfo = this.cloudpilot.getRomInfo(rom);
            if (!rominfo || rominfo.supportedDevices.length === 0) {
                throw new Error('unsupported device');
            }

            deviceId = rominfo.supportedDevices[0];
        }
        this.session = { ...DEFAULT_SESSION, deviceId };

        if (this.emulationService.initWithRom(this.cloudpilot, rom, deviceId, this.session)) {
            this.canvasDisplayService.initialize(undefined, deviceId, this.session.orientation);
            return this;
        }

        throw new Error('failed to initialize session');
    }

    loadSession(session: Uint8Array): this {
        const sessionImage = this.cloudpilot.deserializeSessionImage<SessionMetadata>(session);
        if (!sessionImage) throw new Error('bad session image');

        this.session = {
            hotsyncName: sessionImage.metadata?.hotsyncName,
            speed: sessionImage.metadata?.speed ?? 1,
            orientation: sessionImage.metadata?.deviceOrientation ?? DeviceOrientation.protrait,
            runInBackground: false,
            deviceId: sessionImage.deviceId,
        };

        if (this.emulationService.initWithSessionImage(this.cloudpilot, sessionImage, this.session)) {
            this.canvasDisplayService.initialize(undefined, sessionImage.deviceId, this.session.orientation);
            return this;
        }

        throw new Error('failed to initialize session');
    }

    setCanvas(canvas: HTMLCanvasElement): this {
        this.canvasDisplayService.initialize(canvas, this.session.deviceId, this.session.orientation);

        return this;
    }

    bindInput(pointerTarget: HasEvents, keyEventTarget?: HasEvents): this {
        this.eventHandlingService.bind(pointerTarget, keyEventTarget);

        return this;
    }

    releaseInput(): this {
        this.eventHandlingService.release();

        return this;
    }

    installDatabase(file: Uint8Array): this {
        if (this.cloudpilot.installDb(file) < 0) throw new Error('failed to install database');

        return this;
    }

    installAndLaunchDatabase(file: Uint8Array): this {
        return this.installDatabase(file).launchDatabase(file);
    }

    installFromZipfile(file: Uint8Array): this {
        return this.installFromZipfileAndLaunch(file);
    }

    installFromZipfileAndLaunch(file: Uint8Array, launchFile?: string): this {
        let launch: Uint8Array | undefined;

        this.cloudpilot.withZipfileWalkerSync(file, (walker) => {
            while (walker.GetState() === ZipfileWalkerState.open) {
                const content = walker.GetCurrentEntryContent();
                const name = walker.GetCurrentEntryName();

                if (!content) {
                    throw new Error(`unable to read ${name} from zupfile`);
                }

                this.installDatabase(content);

                if (name.toLowerCase() === launchFile?.toLowerCase()) launch = content.subarray(0, 32).slice();

                walker.Next();
            }

            if (walker.GetState() === ZipfileWalkerState.error) {
                throw new Error('error reading zipfile');
            }
        });

        if (launchFile !== undefined && !launch) throw new Error(`database ${launchFile} not found `);
        if (launch) this.launchDatabase(launch);

        return this;
    }

    launchByName(name: string): this {
        if (!this.cloudpilot.launchAppByName(name)) throw new Error(`failed to launch ${name}`);

        return this;
    }

    launchDatabase(database: Uint8Array): this {
        if (!this.cloudpilot.launchAppByDbHeader(database)) throw new Error('failed to launch database');

        return this;
    }

    reset(): this {
        this.emulationService.reset();

        return this;
    }

    resetNoExtensions(): this {
        this.emulationService.resetNoExtensions();

        return this;
    }

    resetHard(): this {
        this.emulationService.resetHard();

        return this;
    }

    isRunning(): boolean {
        return this.emulationService.isRunning();
    }

    isPowerOff(): boolean {
        return this.emulationService.isPowerOff();
    }

    isUiInitialized(): boolean {
        return this.emulationService.isUiInitialized();
    }

    mute(): this {
        throw new Error('Method not implemented.');

        return this;
    }

    unmute(): this {
        throw new Error('Method not implemented.');

        return this;
    }

    autounmute(): this {
        throw new Error('Method not implemented.');

        return this;
    }

    resume(): this {
        this.emulationService.resume();

        return this;
    }

    pause(): this {
        this.emulationService.pause();

        return this;
    }

    buttonDown(button: Button): this {
        this.emulationService.handleButtonDown(button as number);

        return this;
    }

    buttonUp(button: Button): this {
        this.emulationService.handleButtonUp(button as number);

        return this;
    }

    setSpeed(speed: number): this {
        this.session.speed = speed;

        return this;
    }

    getSpeed(): number {
        return this.session.speed;
    }

    setVolume(volume: number): this {
        throw new Error('Method not implemented.');

        return this;
    }

    getVolume(): number {
        throw new Error('Method not implemented.');
    }

    setOrientation(orientation: DeviceOrientation): this {
        this.session.orientation = orientation;
        this.canvasDisplayService.updateOrientation(this.session.orientation);

        return this;
    }

    getOrientation(): DeviceOrientation {
        return this.session.orientation;
    }

    setHotsyncName(hotsyncName: string | undefined): this {
        this.session.hotsyncName = hotsyncName;

        return this;
    }

    getHotsyncName(): string | undefined {
        return this.session.hotsyncName;
    }

    setRunHidden(toggle: boolean): this {
        this.session.runInBackground = toggle;

        return this;
    }

    getRunHidden(): boolean {
        return this.session.runInBackground;
    }

    private onTimeslice = (): void => {
        if (this.powerOffCached !== this.isPowerOff()) {
            this.powerOffCached = this.isPowerOff();
            this.powerOffChangeEvent.dispatch(this.powerOffCached);
        }
    };

    readonly powerOffChangeEvent = new Event<boolean>();
    readonly isUiInitializedChangeEvent = new Event<boolean>();
    readonly audioInitializedEvent = new Event<void>();
    readonly timesliceEvent: EventInterface<void>;

    private emulationService: EmbeddedEmulationService;
    private canvasDisplayService: EmbeddedCanvasDisplayService;
    private eventHandlingService: GenericEventHandlingService;

    private powerOffCached: boolean;

    private session: Session = { ...DEFAULT_SESSION };
}
