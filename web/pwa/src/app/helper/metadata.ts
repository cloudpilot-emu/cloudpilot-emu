import { Engine } from '@common/model/Engine';
import { SessionMetadata } from '@common/model/SessionMetadata';

import { Session, fixmeAssertSessionHasEngine } from '@pwa/model/Session';

export function metadataForSession(session: Session): SessionMetadata {
    fixmeAssertSessionHasEngine(session, Engine.cloudpilot);

    const { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation } = session;
    return { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation };
}
