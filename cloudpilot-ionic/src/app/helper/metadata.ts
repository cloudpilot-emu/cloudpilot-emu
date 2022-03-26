import { Session } from '../model/Session';
import { SessionMetadata } from '../model/SessionMetadata';

export function metadataForSession(session: Session): SessionMetadata {
    const { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation } = session;

    return { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation };
}
