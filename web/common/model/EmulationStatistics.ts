/**
 * This set of performance statistics can be queries at runtime from the emulator.
 */
export interface EmulationStatistics {
    /**
     * The ratio between the duration of an emulated timeslice and the time that
     * is required by the host to emulate it. If this drops below one the host
     * cannot keep up with the emulation, and Cloudpilot automatically reduces
     * emulation speed.
     */
    hostSpeed: number;

    /**
     * The relative speed of the emulated device. One means full speed, lower
     * values indicate that Cloudpilot reduced speed in order to compensate for
     * a slow host.
     */
    emulationSpeed: number;

    /**
     * The average number of frames per second. Note that only frames are only
     * rendered if the screen content actually changed.
     */
    averageFps: number;
}
