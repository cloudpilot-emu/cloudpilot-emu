import pkg from '../../../../package.json';
import { environment } from '../../environments/environment';
import { REVISION } from '../../revision';

export const VERSION = environment.releaseExtra
    ? `${pkg.version}-${REVISION} (${environment.releaseExtra})`
    : pkg.version;
