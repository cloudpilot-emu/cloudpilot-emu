export interface Version {
    major: number;
    minor: number;
    fix: number;
    stage: number;
    build: number;
}

export function decodeVersion(version: number): Version {
    return {
        major: (version >>> 24) & 0xff,
        minor: (version >>> 20) & 0x0f,
        fix: (version >>> 16) & 0x0f,
        stage: (version >>> 12) & 0x0f,
        build: version & 0xfff,
    };
}
