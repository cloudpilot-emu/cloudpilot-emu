import * as pkg from '../../../package.json';

import { REVISION } from '../../revision';
import { environment } from './../../environments/environment';

export const VERSION = environment.releaseExtra
    ? `${pkg.version}-${REVISION} (${environment.releaseExtra})`
    : pkg.version;
