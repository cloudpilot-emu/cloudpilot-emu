import { NativeAppBackendIos } from './native-app-backend-ios';
import { NativeAppBackendStub } from './native-app-backend-stub';
import { NativeAppBackendTauri } from './native-app-backend-tauri';

export function createNativeAppBackend() {
    if (NativeAppBackendIos.isSupported()) {
        return new NativeAppBackendIos();
    }

    if (NativeAppBackendTauri.isSupported()) {
        return new NativeAppBackendTauri();
    }

    return new NativeAppBackendStub();
}
