import { Session } from '@pwa/model/Session';
import { StorageCard } from '@pwa/model/StorageCard';

export function filenameFragment(prefix: string): string {
    const now = new Date();

    const year = now.getFullYear();
    const month = (now.getMonth() + 1).toString().padStart(2, '0');
    const day = now.getDate().toString().padStart(2, '0');
    const hour = now.getHours().toString().padStart(2, '0');
    const minute = now.getMinutes().toString().padStart(2, '0');
    const second = now.getSeconds().toString().padStart(2, '0');

    return `${prefix}_${year}${month}${day}-${hour}${minute}${second}`;
}

export function filenameForSession(session: Session): string {
    return `${filenameFragment(session.name)}.bin`;
}

export function filenameForSessions(): string {
    return `${filenameFragment('sessions')}.zip`;
}

export function filenameForBackup(session: Session, includeRomDatabases: boolean): string {
    return `backup_${filenameFragment(session.name)}${includeRomDatabases ? '_with_rom' : ''}.zip`;
}

export function filenameForArchive(card: StorageCard): string {
    return `files_${filenameFragment(card.name)}.zip`;
}

export function concatFilenames(files: Array<string>, limit = 3): string {
    if (files.length === 0) return '';

    if (files.length === 1) return files[0];

    return `${files.slice(0, files.length > limit ? limit : files.length - 1).join(', ')} and ${
        files.length > limit ? files.length - limit + ' more files' : files[files.length - 1]
    }`;
}
