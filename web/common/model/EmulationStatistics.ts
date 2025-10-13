/**
 * This set of performance statistics can be queries at runtime from the emulator.
 */
export interface EmulationStatisticsCloudpilot {
    type: 'cloudpilot';

    /**
     * The ratio between the duration of an emulated timeslice and the time that
     * is required by the host to emulate it. If this drops below one the host
     * cannot keep up with the emulation, and CloudpilotEmu automatically reduces
     * emulation speed.
     */
    hostSpeed: number;

    /**
     * The relative speed of the emulated device. One means full speed, lower
     * values indicate that CloudpilotEmu reduced speed in order to compensate for
     * a slow host.
     */
    emulationSpeed: number;

    /**
     * The average number of frames per second. Note that only frames are only
     * rendered if the screen content actually changed.
     */
    averageFps: number;
}

export interface EmulationStatisticsUarm {
    type: 'uarm';

    /**
     * Current emulation speed in MIPS (million instructions per second).
     */
    currentSpeedMips: number;

    /**
     * Current maximum emulation speed without throttling in MIPS (million
     * instructions per second).
     */
    currentMaxSpeedMips: number;
}

export type EmulationStatistics = EmulationStatisticsCloudpilot | EmulationStatisticsUarm;
