import { InjectionToken } from '@angular/core';
import { Cloudpilot } from '@common/bridge/Cloudpilot';

export const TOKEN_EMULATOR_LOCK = new InjectionToken<Lock>('emulator lock');
export const TOKEN_CLOUDPILOT_INSTANCE = new InjectionToken<Promise<Cloudpilot>>('cloudpilot instance');
export const TOKEN_UARM_MODULE = new InjectionToken<Promise<WebAssembly.Module>>('uarm module');
