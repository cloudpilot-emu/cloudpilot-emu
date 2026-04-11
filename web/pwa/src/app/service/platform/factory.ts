import { LifecycleService } from '../lifecycle.service';
import { PlatformBrowser } from './platform-backend-brower';
import { PlatformBackendNativeAppIos } from './platform-backend-native-app-ios';
import { PlatformBackendNativeAppTauri } from './platform-backend-native-app-tauri';

export function createNativeAppBackend(lifecycleService: LifecycleService) {
    if (PlatformBackendNativeAppIos.isSupported()) {
        return new PlatformBackendNativeAppIos();
    }

    if (PlatformBackendNativeAppTauri.isSupported()) {
        return new PlatformBackendNativeAppTauri(lifecycleService);
    }

    return new PlatformBrowser();
}
