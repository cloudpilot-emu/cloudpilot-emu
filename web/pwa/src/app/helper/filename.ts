import { Session } from '@pwa/model/Session';

export function filenameFragment(session: Session): string {
    const now = new Date();

    const year = now.getFullYear();
    const month = (now.getMonth() + 1).toString().padStart(2, '0');
    const day = now.getDate().toString().padStart(2, '0');
    const hour = now.getHours().toString().padStart(2, '0');
    const minute = now.getMinutes().toString().padStart(2, '0');
    const second = now.getSeconds().toString().padStart(2, '0');

    return `${session.name}_${year}${month}${day}-${hour}${minute}${second}`;
}

export function filenameForSession(session: Session): string {
    return `${filenameFragment(session)}.bin`;
}

export function filenameForBackup(session: Session, includeRomDatabases: boolean): string {
    return `backup_${filenameFragment(session)}${includeRomDatabases ? '_with_rom' : ''}.zip`;
}

export function concatFilenames(files: Array<string>, limit = 3): string {
    if (files.length === 0) return '';

    if (files.length === 1) return files[0];

    return `${files.slice(0, files.length > limit ? limit : files.length - 1).join(', ')} and ${
        files.length > limit ? files.length - limit + ' more files' : files[files.length - 1]
    }`;
}
