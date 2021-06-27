import { REVISION } from '../../revision';
import { environment } from './../../environments/environment';
import pkg from '../../../package.json';

export const VERSION = environment.releaseExtra
    ? `${pkg.version}-${REVISION} (${environment.releaseExtra})`
    : pkg.version;
