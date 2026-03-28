import { AlertService } from '../alert.service';
import { NativeAppBackendIos } from './native-app-backend-ios';
import { NativeAppBackendStub } from './native-app-backend-stub';
import { NativeAppBackendTauri } from './native-app-backend-tauri';

export function createNativeAppBackend(alertService: AlertService) {
    if (NativeAppBackendIos.isSupported()) {
        return new NativeAppBackendIos();
    }

    if (NativeAppBackendTauri.isSupported()) {
        return new NativeAppBackendTauri(alertService);
    }

    return new NativeAppBackendStub();
}
