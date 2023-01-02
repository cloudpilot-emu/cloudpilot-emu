export function dirtyPagesSize(imageSize: number): number {
    const pages = (imageSize >>> 13) + (imageSize % 8192 > 0 ? 1 : 0);
    return (pages >>> 3) + (pages % 8 > 0 ? 1 : 0);
}
