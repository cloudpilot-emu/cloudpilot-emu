The "Settings" tab allows you to configure CloudpilotEmu.

# Settings

### Volume

This setting control the volume of the emulated audio. Setting the volume to zero
disables audio.

### Run when hidden

Keep CloudpilotEmu running when its browser tab is hidden. Note that this does not
keep CloudpilotEmu running on mobile if the browser or homescreen app is
backgrounded.

### Clipboard integration (supported browser only)

This turns on clipboard integration. This option is only available on browsers that
support it. Notably, this option is not available on Firefox.

### Network redirection

CloudpilotEmu can connect the virtual PalmOS device to the network via
a proxy server. This configuration supports network hotsync. Please check the
[online documentation](https://github.com/cloudpilot-emu/cloudpilot-emu/blob/master/doc/networking.md)
for detailed instructions.

If you enable network redirection you'll have to enter the address of the proxy
server below. Use the "Test" button to test the connection before attempting to
connect from within the virtual device.

### Auto-lock UI

This setting causes the navigation to automatically hide when the emulated devices
launches on startup.

### Reduce animations

This setting disables several animations that can cause lag on Safari (iOS and MacOS).
It is enabled by default in Safari on MacOS.

### Show Statistics

This overlays the grafitti area with a collection of performance statistics.

### Remote Installation sources

Enabling this will allow you to load ROMs, session images and install files
from remote URLs. Please note that those will have to be served via HTTPS
(unless you are hosting CloudpilotEmu yourself via HTTP), and the file server
has to support CORS.
