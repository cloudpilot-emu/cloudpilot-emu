import { SessionMetadata } from '@common/model/SessionMetadata';

import { Session, fixmeAssertSessionHasEngine } from '@pwa/model/Session';

export function metadataForSession(session: Session): SessionMetadata {
    switch (session.engine) {
        case 'cloudpilot': {
            const { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation } = session;
            return { name, hotsyncName, osVersion, dontManageHotsyncName, speed, deviceOrientation };
        }

        case 'uarm': {
            const { name, osVersion, deviceOrientation, disableAudio, targetMips, warnSlowdownThreshold, maxHostLoad } =
                session;

            return { name, osVersion, deviceOrientation, disableAudio, targetMips, warnSlowdownThreshold, maxHostLoad };
        }

        default:
            throw new Error('unreachable');
    }
    fixmeAssertSessionHasEngine(session, 'cloudpilot');
}
