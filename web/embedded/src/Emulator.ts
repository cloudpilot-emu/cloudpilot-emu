import { Cloudpilot } from '@common/bridge/Cloudpilot';
import { ZipfileWalkerState } from '@common/bridge/ZipfileWalker';
import { Engine } from '@common/engine/Engine';
import { hasDPad } from '@common/helper/deviceProperties';
import { DeviceId } from '@common/model/DeviceId';
import { DeviceOrientation } from '@common/model/DeviceOrientation';
import { EmulationStatistics } from '@common/model/EmulationStatistics';
import { SessionMetadata } from '@common/model/SessionMetadata';
import { EmulatorEventTarget } from '@common/service/GenericEventHandlingService';
import { SkinLoader } from '@common/service/SkinLoader';
import { Mutex } from 'async-mutex';
import { Event as EventImpl } from 'microevent.ts';

import { CloudpilotEvent } from './Event';
import { SerialPort } from './SerialPort';
import { Watcher } from './Watcher';
import { Button } from './index';
import { Session } from './model/Session';
import { EmbeddedAudioService } from './service/EmbeddedAudioService';
import { EmbeddedCanvasDisplayService } from './service/EmbeddedCanvasDisplayService';
import { EmbeddedEmulationService } from './service/EmbeddedEmulationService';
import { EmbeddedEventHandlingService } from './service/EmbeddedEventHandlingService';

const DEFAULT_SESSION: Session = {
    hotsyncName: undefined,
    orientation: DeviceOrientation.portrait,
    runInBackground: false,
    speed: 1,
    deviceId: DeviceId.m515,
    maxHostLoad: 1,
    disablePcmAudio: false,
    disableDpad: false,
};

const CARD_KEY = 'MEMORY_CARD';

function assertEngineInitialized(engine: Engine | undefined): asserts engine is Engine {
    if (!engine) throw new Error('session not initialized');
}

// TODO: D-Pad
// TODO: uARM

/**
 * The main emulator interface. Most methods that interact with the emulator are async and
 * should be awaited.
 */
export interface Emulator {
    /**
     * Load a ROM and put the emulator in paused state.
     *
     * @param rom Device ROM
     * @param nand Device NAND. OS5 devices only.
     * @param deviceId Optional: device ID, autodetected if not specified
     */
    loadRom(rom: Uint8Array, nand?: Uint8Array, deviceId?: DeviceId): Promise<void>;

    /**
     * Load a Cloudpilot session and put the emulator in paused state.
     *
     * @param session Session image
     */
    loadSession(session: Uint8Array): Promise<void>;

    /**
     * Attach and mount a gzip compressed card image.
     *
     * @param cardImage Gzip compressed image data
     */
    insertCompressedCardImage(cardImage: Uint8Array): Promise<void>;

    /**
     * Attach and mount a plain card image.
     *
     * @param cardImage Image data
     */
    insertCardImage(cardImage: Uint8Array): Promise<void>;

    /**
     * Eject a previously inserted card image:
     */
    ejectCard(): Promise<void>;

    /**
     * Check whether a card is currently mounted.
     */
    isCardMounted(): Promise<boolean>;

    /**
     * Configure the canvas element used for displaying the emulator.
     *
     * @param canvas Canvas for displaying the emulator
     */
    setCanvas(canvas: HTMLCanvasElement): void;

    /**
     * Release the canvas element.
     */
    releaseCanvas(): void;

    /**
     * Receive input events from the specified sources. If this method is called
     * multiple times the previous sources will be unbound.
     *
     * @param keyboardTarget Optional: target for keyboard events, default: `window`
     */
    bindInput(keyboardTarget?: EmulatorEventTarget): void;

    /**
     * Unbind the handlers previous bound with `bindInput`.
     */
    releaseInput(): void;

    /**
     * Install a prc or pdb database to the device.
     *
     * @param file The database data.
     */
    installDatabase(file: Uint8Array): Promise<void>;

    /**
     * Install a prc database to the device and attempt to launch it.
     *
     * @param file The database data.
     */
    installAndLaunchDatabase(file: Uint8Array): Promise<void>;

    /**
     * Extract all databases from a zip archive and install them.
     *
     * @param file The zip archive data.
     */
    installFromZipfile(file: Uint8Array): Promise<void>;

    /**
     * Extract all databases from a zip archive and install them, then attempt to
     * launch the specified file.
     *
     * @param file The zip archive data.
     * @param launchFile The file name of the database that Cloudpilot will try to launch.
     */
    installFromZipfileAndLaunch(file: Uint8Array, launchFile: string): Promise<void>;

    /**
     * Attempt to launch the database with the specified name.
     *
     * @param name Database name
     */
    launchByName(name: string): Promise<void>;

    /**
     * Attempt to extract the name from a database and launch it.
     *
     * @param database Database data (only the first 32 bytes are required)
     */
    launchDatabase(database: Uint8Array): Promise<void>;

    /**
     * Perform a soft reset (equivalent of pushing the reset button).
     */
    reset(): Promise<void>;

    /**
     * Reset w/o system extensions (equivalent to holding "down" while pushing the
     * reset button).
     */
    resetNoExtensions(): Promise<void>;

    /**
     * Hard reset (equivalent to holding "power" while pushing the
     * reset button).
     */
    resetHard(): Promise<void>;

    /**
     * Is the emulator running?
     */
    isRunning(): Promise<boolean>;

    /**
     * Is the device powered off?
     */
    isPowerOff(): Promise<boolean>;

    /**
     * Has the emulated device passed UI initialization (during boot)? This
     * is required before software can be installed.
     */
    isUiInitialized(): Promise<boolean>;

    /**
     * Resume a paused device.
     */
    resume(): Promise<void>;

    /**
     * Pause a running device.
     */
    pause(): Promise<void>;

    /**
     * Push a hardware button.
     *
     * @param button The desired button
     */
    buttonDown(button: Button): void;

    /**
     * Release a hardware button.
     *
     * @param button The desired button
     */

    buttonUp(button: Button): void;

    /**
     * Adjust speed of the emulated device.
     *
     * @param speed Speed factor
     */
    setSpeed(speed: number): void;

    /**
     * Query configured speed factor.
     */
    getSpeed(): number;

    /**
     * Set audio volume.
     *
     * @param volume Volume (1 = 100%, 0 = silent)
     */
    setVolume(volume: number): void;

    /**
     * Query audio volume
     */
    getVolume(): number;

    /**
     * Disable PCM audio on OS5. This will improve emulation speed (by freeing cycles
     * that would be used to generate audio) but may lead to * compatibility issues with
     * some apps.
     *
     * @param disablePcmAudio Disable / enable PCM audio
     */
    setDisablePcmAudio(disablePcmAudio: boolean): void;

    /**
     * Query whether PCM audio is disabled.
     */
    getDisablePcmAudio(): boolean;

    /**
     * Set the maximum host load. This applies to OS5 emulation only.
     *
     * @param maxHostLoad Maximum host load (1 = full core)
     */
    setMaxHostLoad(maxHostLoad: number): void;

    /**
     * Get the maximum host load.
     */
    getMaxHostLoad(): number;

    /**
     * Disable the full d-pad on devices that support it.
     *
     * @param disableDpad  Disable / enable d-pad
     */
    setDisableDpad(disableDpad: boolean): void;

    /**
     * Query whether the d-pad has been disabled.
     */
    getDisableDpad(): boolean;

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
    setGameMode(gameModeActive: boolean): void;

    /**
     * Is game mode enabled?
     */
    isGameMode(): boolean;

    /**
     * Enable or disable shift-ctrl for toggling game mode (enabled by default).
     *
     * @param enableGamemodeHotkey Desired state
     */
    setGameModeHotkeyEnabled(enableGamemodeHotkey: boolean): void;

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
    setGameModeIndicatorEnabled(gameModeIndicatorEnabled: boolean): void;

    /**
     * Is game mode overlay enabled?
     */
    isGameModeIndicatorEnabled(): boolean;

    /**
     * Change device orientation.
     *
     * @param orientation Desired orientation
     */
    setOrientation(orientation: DeviceOrientation): void;

    /**
     * Query device orientation
     */
    getOrientation(): DeviceOrientation;

    /**
     * Set hotsync name.
     *
     * @param hotsyncName Desired hotsync name
     */
    setHotsyncName(hotsyncName: string | undefined): Promise<void>;

    /**
     * Get hotsync name.
     */
    getHotsyncName(): Promise<string | undefined>;

    /**
     * Keep running if the emulator tab is not visible?
     *
     * @param toggle Desired state
     */
    setRunHidden(toggle: boolean): void;

    /**
     * Keep running if the emulator tab is not visible?
     */
    getRunHidden(): boolean;

    /**
     * Get performance statistics
     */
    getStatistics(): EmulationStatistics | undefined;

    /**
     * Get serial transport for IR transceiver.
     */
    getSerialPortIR(): SerialPort | undefined;

    /**
     * Get serial transport for serial port.
     */
    getSerialPortSerial(): SerialPort | undefined;

    /**
     * Fires when the device turns on or off.
     */
    readonly powerOffChangeEvent: CloudpilotEvent<boolean>;

    /**
     * Fires when PalmOS resets or passed UI initialization during boot.
     */
    readonly isUiInitializedChangeEvent: CloudpilotEvent<boolean>;

    /**
     * Fires when audio is initializd successfully.
     */
    readonly audioInitializedEvent: CloudpilotEvent<void>;

    /**
     * Fires after each emulated timeslice (typically 60 times per second)
     */
    readonly timesliceEvent: CloudpilotEvent<void>;

    /**
     * Fires when the hotsync name changes. This does not happen immediatelly when
     * `setHotsyncName` is called, but only when the OS is notified of the new name.
     */
    readonly hotsyncNameChangeEvent: CloudpilotEvent<string>;

    /**
     * Fires if game mode is enabled or disabled.
     */
    readonly gameModeChangeEvent: CloudpilotEvent<boolean>;
}

export class EmulatorImpl implements Emulator {
    constructor(
        private cloudpilot: Cloudpilot,
        uarmModuleFactory: () => Promise<WebAssembly.Module>,
        uarmWorkerUrl?: string,
        pcmWorkletUrl?: string,
    ) {
        this.emulationService = new EmbeddedEmulationService(cloudpilot, uarmModuleFactory, uarmWorkerUrl);
        this.canvasDisplayService = new EmbeddedCanvasDisplayService(new SkinLoader(Promise.resolve(cloudpilot)));
        this.eventHandlingService = new EmbeddedEventHandlingService(this.emulationService, this.canvasDisplayService);
        this.audioService = new EmbeddedAudioService(this.session, this.emulationService, pcmWorkletUrl);

        this.emulationService.newFrameEvent.addHandler((canvas) =>
            this.canvasDisplayService.updateEmulationCanvas(canvas),
        );

        this.timesliceEvent.addHandler(this.onTimeslice);

        this.powerOffWatcher = new Watcher(() => this.emulationService.isPowerOff());
        this.uiInitializedWatcher = new Watcher(() => this.emulationService.isUiInitialized());
        this.hotsyncNameWatcher = new Watcher(() => this.session.hotsyncName ?? '');
    }

    getStatistics(): EmulationStatistics | undefined {
        return this.emulationService.getStatistics();
    }

    async loadRom(rom: Uint8Array, nand?: Uint8Array, deviceId?: DeviceId): Promise<void> {
        await this.mutex.runExclusive(async () => {
            if (deviceId === undefined) {
                const rominfo = this.cloudpilot.getRomInfo(rom);
                if (!rominfo || rominfo.supportedDevices.length === 0) {
                    throw new Error('unsupported device');
                }

                deviceId = rominfo.supportedDevices[0];
            }
            this.session = { ...DEFAULT_SESSION, deviceId };
            this.audioService.setSession(this.session);

            if (!(await this.emulationService.initWithRom(rom.slice(), nand?.slice(), deviceId, this.session))) {
                throw new Error('failed to initialize session');
            }

            this.eventHandlingService.setDpadEnabled(this.enableDpad());
            this.canvasDisplayService.setDpadEnabled(this.enableDpad());
            await this.canvasDisplayService.initialize(undefined, deviceId, this.session.orientation);
        });
    }

    async loadSession(session: Uint8Array): Promise<void> {
        await this.mutex.runExclusive(async () => {
            const sessionImage = this.cloudpilot.deserializeSessionImage<SessionMetadata>(session);
            if (!sessionImage) throw new Error('bad session image');

            this.session = {
                hotsyncName: sessionImage.metadata?.hotsyncName,
                speed: sessionImage.metadata?.speed ?? 1,
                orientation: sessionImage.metadata?.deviceOrientation ?? DeviceOrientation.portrait,
                runInBackground: false,
                deviceId: sessionImage.deviceId,
                maxHostLoad: sessionImage.metadata?.maxHostLoad ?? 1,
                disablePcmAudio: sessionImage.metadata?.disableAudio ?? false,
                disableDpad: false,
            };
            this.audioService.setSession(this.session);

            if (!(await this.emulationService.initWithSessionImage(sessionImage, this.session))) {
                throw new Error('failed to initialize session');
            }

            this.eventHandlingService.setDpadEnabled(this.enableDpad());
            this.canvasDisplayService.setDpadEnabled(this.enableDpad());
            await this.canvasDisplayService.initialize(undefined, sessionImage.deviceId, this.session.orientation);
        });
    }

    async insertCompressedCardImage(cardImage: Uint8Array): Promise<void> {
        const decompressedImage = this.cloudpilot.decompress(cardImage);
        if (!decompressedImage) {
            throw new Error('failed to decompress and mount card image');
        }

        await this.insertCardImage(decompressedImage);
    }

    async insertCardImage(cardImage: Uint8Array): Promise<void> {
        await this.mutex.runExclusive(async () => {
            const engine = this.getEngine();

            if (cardImage.length % 512 !== 0) throw new Error('card image size must be a multiple of 512');

            if (await this.isCardMountedUnguarded()) throw new Error('card already mounted');

            const allocatedImage = engine.allocateCard(CARD_KEY, cardImage.length);
            allocatedImage.set(new Uint32Array(cardImage.buffer, cardImage.byteOffset, cardImage.byteLength >>> 2));

            if (!(await engine.mountCard(CARD_KEY))) throw new Error('failed to mount card image');
        });
    }

    async ejectCard(): Promise<void> {
        await this.mutex.runExclusive(async () => {
            const engine = this.getEngine();

            if (!(await this.isCardMountedUnguarded())) return;

            await engine.releaseCard(CARD_KEY);
        });
    }

    async isCardMounted(): Promise<boolean> {
        return await this.mutex.runExclusive(() => this.isCardMountedUnguarded());
    }

    setCanvas(canvas: HTMLCanvasElement): void {
        this.canvas = canvas;
        void this.canvasDisplayService.initialize(canvas, this.session.deviceId, this.session.orientation);
    }

    releaseCanvas(): void {
        this.canvas = undefined;
        void this.canvasDisplayService.initialize(undefined, this.session.deviceId, this.session.orientation);
    }

    bindInput(keyEventTarget?: EmulatorEventTarget): void {
        if (!this.canvas) {
            throw new Error('you must set up the canvas setCanvas before calling bindInput');
        }
        this.eventHandlingService.bind(this.canvas, this.enableDpad(), keyEventTarget);
    }

    releaseInput(): void {
        this.eventHandlingService.release();
    }

    async installDatabase(file: Uint8Array): Promise<void> {
        await this.mutex.runExclusive(() => this.installDatabaseUnguarded(file));
    }

    async installAndLaunchDatabase(file: Uint8Array): Promise<void> {
        await this.installDatabase(file);
        await this.launchDatabase(file);
    }

    async installFromZipfile(file: Uint8Array): Promise<void> {
        await this.installFromZipfileAndLaunch(file);
    }

    async installFromZipfileAndLaunch(file: Uint8Array, launchFile?: string): Promise<void> {
        await this.mutex.runExclusive(async () => {
            let launch: Uint8Array | undefined;

            await this.cloudpilot.withZipfileWalker(file, async (walker) => {
                while (walker.GetState() === ZipfileWalkerState.open) {
                    const content = walker.GetCurrentEntryContent();
                    const name = walker.GetCurrentEntryName();

                    if (!content) {
                        throw new Error(`unable to read ${name} from zipfile`);
                    }

                    await this.installDatabaseUnguarded(content);

                    if (name.toLowerCase() === launchFile?.toLowerCase()) launch = content.subarray(0, 32).slice();

                    walker.Next();
                }

                if (walker.GetState() === ZipfileWalkerState.error) {
                    throw new Error('error reading zipfile');
                }
            });

            if (launchFile !== undefined && !launch) throw new Error(`database ${launchFile} not found `);
            if (launch) await this.launchDatabaseUnguarded(launch);
        });
    }

    async launchByName(name: string): Promise<void> {
        await this.mutex.runExclusive(async () => {
            const engine = this.getEngine();

            if (!(await engine.launchAppByName(name))) throw new Error(`failed to launch ${name}`);
        });
    }

    async launchDatabase(database: Uint8Array): Promise<void> {
        await this.mutex.runExclusive(() => this.launchDatabaseUnguarded(database));
    }

    async reset(): Promise<void> {
        await this.mutex.runExclusive(() => this.emulationService.reset());
    }

    async resetNoExtensions(): Promise<void> {
        await this.mutex.runExclusive(() => this.emulationService.resetNoExtensions());
    }

    async resetHard(): Promise<void> {
        await this.mutex.runExclusive(() => this.emulationService.resetHard());
    }

    async isRunning(): Promise<boolean> {
        return await this.mutex.runExclusive(() => this.emulationService.isRunning());
    }

    async isPowerOff(): Promise<boolean> {
        return await this.mutex.runExclusive(() => this.emulationService.isPowerOff());
    }

    async isUiInitialized(): Promise<boolean> {
        return await this.mutex.runExclusive(() => this.emulationService.isUiInitialized());
    }

    async resume(): Promise<void> {
        await this.mutex.runExclusive(() => this.emulationService.resume());
    }

    async pause(): Promise<void> {
        await this.mutex.runExclusive(() => this.emulationService.pause());
    }

    buttonDown(button: Button): void {
        this.emulationService.handleButtonDown(button as number);
    }

    buttonUp(button: Button): void {
        this.emulationService.handleButtonUp(button as number);
    }

    setSpeed(speed: number): void {
        this.session.speed = speed;
        this.emulationService.syncSettings();
    }

    getSpeed(): number {
        return this.session.speed;
    }

    setVolume(volume: number): void {
        this.audioService.setVolume(volume);
    }

    getVolume(): number {
        return this.audioService.getVolume();
    }

    setDisablePcmAudio(disablePcmAudio: boolean): void {
        this.session.disablePcmAudio = disablePcmAudio;
        this.emulationService.syncSettings();
    }

    getDisablePcmAudio(): boolean {
        return this.session.disablePcmAudio;
    }

    setMaxHostLoad(maxHostLoad: number): void {
        this.session.maxHostLoad = maxHostLoad;
        this.emulationService.syncSettings();
    }

    getMaxHostLoad(): number {
        return this.session.maxHostLoad;
    }

    setDisableDpad(disableDpad: boolean): void {
        this.session.disableDpad = disableDpad;

        this.eventHandlingService.setDpadEnabled(this.enableDpad());
        this.canvasDisplayService.setDpadEnabled(this.enableDpad());
        void this.canvasDisplayService.initialize();
    }

    getDisableDpad(): boolean {
        return this.session.disableDpad;
    }

    async initializeAudio(): Promise<boolean> {
        await this.audioService.initialize();
        return this.audioService.isInitialized();
    }

    isAudioInitialized(): boolean {
        return this.audioService.isInitialized();
    }

    setOrientation(orientation: DeviceOrientation): void {
        this.session.orientation = orientation;
        this.canvasDisplayService.updateOrientation(this.session.orientation);
    }

    getOrientation(): DeviceOrientation {
        return this.session.orientation;
    }

    async setHotsyncName(hotsyncName: string | undefined): Promise<void> {
        await this.mutex.runExclusive(() => {
            this.session.hotsyncName = hotsyncName;
            this.emulationService.syncSettings();
        });
    }

    async getHotsyncName(): Promise<string | undefined> {
        return this.mutex.runExclusive(() => this.session.hotsyncName);
    }

    setRunHidden(toggle: boolean): void {
        this.session.runInBackground = toggle;
        this.emulationService.syncSettings();
    }

    getRunHidden(): boolean {
        return this.session.runInBackground;
    }

    isGameMode(): boolean {
        return this.eventHandlingService.isGameMode();
    }

    setGameMode(gameModeActive: boolean): void {
        this.eventHandlingService.setGameMode(gameModeActive);
    }

    isGameModeHotkeyEnabled(): boolean {
        return this.eventHandlingService.isGameModeHotkeyEnabled();
    }

    setGameModeHotkeyEnabled(enableGamemodeHotkey: boolean): void {
        this.eventHandlingService.setGameModeHotkeyEnabled(enableGamemodeHotkey);
    }

    isGameModeIndicatorEnabled(): boolean {
        return this.canvasDisplayService.isGameModeIndicatorEnabled();
    }

    setGameModeIndicatorEnabled(gameModeIndicatorEnabled: boolean): void {
        this.canvasDisplayService.setGameModeIndicatorEnabled(gameModeIndicatorEnabled);
    }

    getSerialPortIR(): SerialPort | undefined {
        return this.emulationService.getSerialPortIR();
    }

    getSerialPortSerial(): SerialPort | undefined {
        return this.emulationService.getSerialPortSerial();
    }

    get powerOffChangeEvent(): CloudpilotEvent<boolean> {
        return this.powerOffWatcher.changeEvent;
    }

    get isUiInitializedChangeEvent(): CloudpilotEvent<boolean> {
        return this.uiInitializedWatcher.changeEvent;
    }

    get hotsyncNameChangeEvent(): CloudpilotEvent<string> {
        return this.hotsyncNameWatcher.changeEvent;
    }

    get gameModeChangeEvent(): CloudpilotEvent<boolean> {
        return this.eventHandlingService.gameModeChangeEvent;
    }

    get timesliceEvent(): CloudpilotEvent<void> {
        return this.emulationService.timesliceEvent;
    }

    private onTimeslice = (): void => {
        this.powerOffWatcher.update();
        this.uiInitializedWatcher.update();
        this.hotsyncNameWatcher.update();
    };

    private async isCardMountedUnguarded(): Promise<boolean> {
        const engine = this.emulationService.getEngine();
        if (!engine) return false;

        return (await engine.getMountedKey()) === CARD_KEY;
    }

    private async installDatabaseUnguarded(file: Uint8Array) {
        const engine = this.getEngine();

        if ((await engine.installDb(file)) < 0) throw new Error('failed to install database');
    }

    private async launchDatabaseUnguarded(database: Uint8Array) {
        const engine = this.getEngine();

        if (!(await engine.launchAppByHeader(database))) throw new Error(`failed to launch ${name}`);
    }

    private getEngine(): Engine {
        const engine = this.emulationService.getEngine();
        assertEngineInitialized(engine);

        return engine;
    }

    private enableDpad(): boolean {
        return !this.session.disableDpad && hasDPad(this.session.deviceId);
    }

    readonly audioInitializedEvent = new EventImpl<void>();

    private mutex = new Mutex();

    private emulationService: EmbeddedEmulationService;
    private canvasDisplayService: EmbeddedCanvasDisplayService;
    private eventHandlingService: EmbeddedEventHandlingService;
    private audioService: EmbeddedAudioService;

    private powerOffWatcher: Watcher<boolean>;
    private uiInitializedWatcher: Watcher<boolean>;
    private hotsyncNameWatcher: Watcher<string>;

    private session: Session = { ...DEFAULT_SESSION };

    private canvas: HTMLCanvasElement | undefined;
}
