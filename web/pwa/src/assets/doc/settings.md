The "Settings" tab allows you to configure CloudpilotEmu.

# Emulation

### Volume

This setting control the volume of the emulated audio.

### Enable audio on start

Enable audio when the app starts.
:span[Note that this does not work 100% reliable on all platforms.]{.no-feature-native-app}

:::div{.feature-run-hidden}
### Run when hidden

Keep CloudpilotEmu running when its browser tab is hidden.
:::

# Host integration

:::div{.feature-clipboard-integration}
### Clipboard integration

This turns on clipboard integration.
:::

### Network redirection

CloudpilotEmu can connect the virtual PalmOS device to the network via
a proxy server. This configuration supports network hotsync. Please check the
[online documentation](https://github.com/cloudpilot-emu/cloudpilot-emu/blob/master/doc/networking.md)
for detailed instructions.

If you enable network redirection you'll have to enter the address of the proxy
server below. Use the "Test" button to test the connection before attempting to
connect from within the virtual device.

# User interface

### Auto-lock UI

This setting causes the navigation to automatically hide when the emulated devices
launches on startup.

### Reduce animations

This setting disables several animations that can cause lag on Safari (iOS and MacOS).

### Show Statistics

This overlays the grafitti area with a collection of performance statistics.

### Open from remote sources

Enabling this will allow you to load ROMs, session images and install files
from remote URLs. Please note that those will have to be served via HTTPS
(unless you are hosting CloudpilotEmu yourself via HTTP), and the file server
has to support CORS.

:::div{.feature-indicator-fix}
### Fix iOS home indicator (iOS Safari only)

Safari on iOS has a glitch that may display the home indicator (the white-ish bar
at the bottom of the screen) on top of the UI instead below it. Whether you are
affected depends on how the toolbar is configured in the Safari settings. This
is cannot be detected, but you can change this setting in order to work around
the issue if you are affected.
:::