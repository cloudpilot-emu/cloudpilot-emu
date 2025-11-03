import { InjectionToken } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';

export const EMULATOR_LOCK_TOKEN = new InjectionToken<Lock>('emulator lock');
export const CLOUDPILOT_INSTANCE_TOKEN = new InjectionToken<Promise<Cloudpilot>>('cloudpilot instance');
