import { Session } from '@pwa/model/Session';
import { SessionMetadata } from '@common/model/SessionMetadata';

export function metadataForSession(session: Session): SessionMetadata {
    const { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation } = session;

    return { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation };
}
