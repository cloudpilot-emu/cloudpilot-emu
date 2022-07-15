import * as skin from '@common/skin';

import { Cloudpilot, ZipfileWalkerState } from '@common/Cloudpilot';

import { Button } from './index';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { EmbeddedAudioService } from './service/EmbeddedAudioService';
import { EmbeddedCanvasDisplayService } from './service/EmbeddedCanvasDisplayService';
import { EmbeddedEmulationService } from './service/EmbeddedEmulationService';
import { EmbeddedEventHandlingServie } from './service/EmbeddedEventHandlingService';
import { EmulationStatistics } from '@common/model/EmulationStatistics';
import { Event } from './Event';
import { Event as EventImpl } from 'microevent.ts';
import { EventSource } from '@common/service/GenericEventHandlingService';
import { Session } from './model/Session';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { Watcher } from './Watcher';
import { loadSkin } from '@common/skin/loader';

const DEFAULT_SESSION: Session = {
    hotsyncName: undefined,
    orientation: DeviceOrientation.portrait,
    runInBackground: false,
    speed: 1,
    deviceId: DeviceId.m515,
};

export interface Emulator {
    /**
     * Load a ROM and put the emulator in paused state.
     *
     * @param rom Device ROM
     * @param deviceId Optional: device ID, autodetected if not specified
     */
    loadRom(rom: Uint8Array, deviceId?: DeviceId): this;

    /**
     * Load a Cloudpilot session and put the emulator in paused state.
     *
     * @param session Session image
     */
    loadSession(session: Uint8Array): this;

    /**
     * Configure the canvas element used for displaying the emulator.
     *
     * @param canvas Canvas for displaying the emulator
     */
    setCanvas(canvas: HTMLCanvasElement): this;

    /**
     * Receive input events from the specified sources. If this method is called
     * multiple times the previous sources will be unbound.
     *
     * @param pointerSource Target for pointer events
     * @param keyboardSource Optional: target for keyboard events, default: `window`
     */
    bindInput(pointerSource: EventSource, keyboardSource?: EventSource): this;

    /**
     * Unbind the handlers previous bound with `bindInput`.
     */
    releaseInput(): this;

    /**
     * Install a prc or pdb database to the device.
     *
     * @param file The database data.
     */
    installDatabase(file: Uint8Array): this;

    /**
     * Install a prc database to the device and attempt to launch it.
     *
     * @param file The database data.
     */
    installAndLaunchDatabase(file: Uint8Array): this;

    /**
     * Extract all databases from a zip archive and install them.
     *
     * @param file The zip archive data.
     */
    installFromZipfile(file: Uint8Array): this;

    /**
     * Extract all databases from a zip archive and install them, then attampt to
     * launch the specified file.
     *
     * @param file The zip archive data.
     */
    installFromZipfileAndLaunch(file: Uint8Array, launchFile: string): this;

    /**
     * Attemot to launch the database with the specified name.
     *
     * @param name Database name
     */
    launchByName(name: string): this;

    /**
     * Attempt to extract the name from a database and launch it.
     *
     * @param database Database data (only the first 32 bytes are required)
     */
    launchDatabase(database: Uint8Array): this;

    /**
     * Perform a soft reset (equivalent of pushing the reset button).
     */
    reset(): this;

    /**
     * Reset w/o system extensions (equivalent to holding "down" while pushing the
     * reset button).
     */
    resetNoExtensions(): this;

    /**
     * Hard reset (equivalent to holding "power" while pushing the
     * reset button).
     */
    resetHard(): this;

    /**
     * Is the emulator running?
     */
    isRunning(): boolean;

    /**
     * Is the device powered off?
     */
    isPowerOff(): boolean;

    /**
     * Has the emulated device passed UI initialization (during boot)? This
     * is required before software can be installed.
     */
    isUiInitialized(): boolean;

    /**
     * Resume a paused device.
     */
    resume(): this;

    /**
     * Pause a running device.
     */
    pause(): this;

    /**
     * Push a hardware button.
     *
     * @param button The desired button
     */
    buttonDown(button: Button): this;

    /**
     * Release a hardware button.
     *
     * @param button The desired button
     */

    buttonUp(button: Button): this;

    /**
     * Adjust speed of the emulated device.
     *
     * @param speed Speed factor
     */
    setSpeed(speed: number): this;

    /**
     * Query configured speed factor.
     */
    getSpeed(): number;

    /**
     * Set audio volume.
     *
     * @param volume Volume (1 = 100%, 0 = silent)
     */
    setVolume(volume: number): this;

    /**
     * Query audio volume
     */
    getVolume(): number;

    /**
     * Initialize audio. This must be called from an event handler that was triggered
     * by a user interaction, i.e. a click or a key press.
     */
    initializeAudio(): Promise<boolean>;

    /**
     * Was audio initialized succesfully?
     */
    isAudioInitialized(): boolean;

    /**
     * Enable or disable game mode (direct key mapping to hardware buttons).
     *
     * @param gameModeActive Desired state
     */
    setGameMode(gameModeActive: boolean): this;

    /**
     * Is game mode enabled?
     */
    isGameMode(): boolean;

    /**
     * Enable or disable shift-ctrl for toggling game mode (enabled by default).
     *
     * @param enableGamemodeHotkey Desired state
     */
    setGameModeHotkeyEnabled(enableGamemodeHotkey: boolean): this;

    /**
     * Can game mode be toggled via shift-ctrl?
     */
    isGameModeHotkeyEnabled(): boolean;

    /**
     * Enable or disable game mode indicator (overlays hard buttons if game mode is active)? Enabled
     * by default.
     *
     * @param gameModeIndicatorEnabled Desired state
     */
    setGameModeIndicatorEnabled(gameModeIndicatorEnabled: boolean): this;

    /**
     * Is game mode overlay enabled?
     */
    isGameModeIndicatorEnabled(): boolean;

    /**
     * Change device orientation.
     *
     * @param orientation Desired orientation
     */
    setOrientation(orientation: DeviceOrientation): this;

    /**
     * Query device orientation
     */
    getOrientation(): DeviceOrientation;

    /**
     * Set hotsync name.
     *
     * @param hotsyncName Desired hotsync name
     */
    setHotsyncName(hotsyncName: string | undefined): this;

    /**
     * Get hotsync name.
     */
    getHotsyncName(): string | undefined;

    /**
     * Keep running if the emulator tab is not visible?
     *
     * @param toggle Desired state
     */
    setRunHidden(toggle: boolean): this;

    /**
     * Keep running if the emulator tab is not visible?
     */
    getRunHidden(): boolean;

    /**
     * Get performance statistics
     */
    getStatistics(): EmulationStatistics;

    /**
     * Fires when the device turns on or off.
     */
    readonly powerOffChangeEvent: Event<boolean>;

    /**
     * Fires when PalmOS resets or passed UI initialization during boot.
     */
    readonly isUiInitializedChangeEvent: Event<boolean>;

    /**
     * Fires when audio is initializd successfully.
     */
    readonly audioInitializedEvent: Event<void>;

    /**
     * Fires after each emulated timeslice (typicall 60 times per second)
     */
    readonly timesliceEvent: Event<void>;

    /**
     * Fires when the hotsync name changes. This does not happen immediatelly when
     * `setHotsyncName` is called, but only when the OS is notified of the new name.
     */
    readonly hotsyncNameChangeEvent: Event<string>;

    /**
     * Fires if game mode is enabled or disabled.
     */
    readonly gameModeChangeEvent: Event<boolean>;
}

export class EmulatorImpl implements Emulator {
    constructor(private cloudpilot: Cloudpilot) {
        this.emulationService = new EmbeddedEmulationService();
        this.canvasDisplayService = new EmbeddedCanvasDisplayService(loadSkin(Promise.resolve(skin)));
        this.eventHandlingService = new EmbeddedEventHandlingServie(this.emulationService, this.canvasDisplayService);
        this.audioService = new EmbeddedAudioService(this.emulationService);

        this.emulationService.newFrameEvent.addHandler((canvas) =>
            this.canvasDisplayService.updateEmulationCanvas(canvas)
        );

        this.timesliceEvent.addHandler(this.onTimeslice);

        this.powerOffWatcher = new Watcher(() => this.isPowerOff());
        this.uiInitializedWatcher = new Watcher(() => this.isUiInitialized());
        this.hotsyncNameWatcher = new Watcher(() => this.getHotsyncName() || '');
    }

    getStatistics(): EmulationStatistics {
        return this.emulationService.getStatistics();
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
            orientation: sessionImage.metadata?.deviceOrientation ?? DeviceOrientation.portrait,
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

    bindInput(pointerTarget: EventSource, keyEventTarget?: EventSource): this {
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
        this.audioService.setVolume(volume);

        return this;
    }

    getVolume(): number {
        return this.audioService.getVolume();
    }

    async initializeAudio(): Promise<boolean> {
        await this.audioService.initialize();
        return this.audioService.isInitialized();
    }

    isAudioInitialized(): boolean {
        return this.audioService.isInitialized();
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

    isGameMode(): boolean {
        return this.eventHandlingService.isGameMode();
    }

    setGameMode(gameModeActive: boolean): this {
        this.eventHandlingService.setGameMode(gameModeActive);

        return this;
    }

    isGameModeHotkeyEnabled(): boolean {
        return this.eventHandlingService.isGameModeHotkeyEnabled();
    }

    setGameModeHotkeyEnabled(enableGamemodeHotkey: boolean): this {
        this.eventHandlingService.setGameModeHotkeyEnabled(enableGamemodeHotkey);

        return this;
    }

    isGameModeIndicatorEnabled(): boolean {
        return this.canvasDisplayService.isGameModeIndicatorEnabled();
    }

    setGameModeIndicatorEnabled(gameModeIndicatorEnabled: boolean): this {
        this.canvasDisplayService.setGameModeIndicatorEnabled(gameModeIndicatorEnabled);

        return this;
    }

    get powerOffChangeEvent(): Event<boolean> {
        return this.powerOffWatcher.changeEvent;
    }

    get isUiInitializedChangeEvent(): Event<boolean> {
        return this.uiInitializedWatcher.changeEvent;
    }

    get hotsyncNameChangeEvent(): Event<string> {
        return this.hotsyncNameWatcher.changeEvent;
    }

    get gameModeChangeEvent(): Event<boolean> {
        return this.eventHandlingService.gameModeChangeEvent;
    }

    get timesliceEvent(): Event<void> {
        return this.emulationService.timesliceEvent;
    }

    private onTimeslice = (): void => {
        this.powerOffWatcher.update();
        this.uiInitializedWatcher.update();
        this.hotsyncNameWatcher.update();
    };

    readonly audioInitializedEvent = new EventImpl<void>();

    private emulationService: EmbeddedEmulationService;
    private canvasDisplayService: EmbeddedCanvasDisplayService;
    private eventHandlingService: EmbeddedEventHandlingServie;
    private audioService: EmbeddedAudioService;

    private powerOffWatcher: Watcher<boolean>;
    private uiInitializedWatcher: Watcher<boolean>;
    private hotsyncNameWatcher: Watcher<string>;

    private session: Session = { ...DEFAULT_SESSION };
}
