This document describes how network access in CloudpilotEmu is set up and
how the virtual device is configured for Network Hotsync.

# Basic Setup

As a web application, CloudpilotEmu cannot access the network directly. In order
to access the network you need to run a small proxy server locally and
configure CloudpilotEmu to use it. As the official version of CloudpilotEmu is
hosted over HTTPS, the connection between the proxy server and CloudpilotEmu
has to be encrypted as well.

The necessary steps to set up networking are

1. Download latest server version from the
   [releases page](https://github.com/cloudpilot-emu/cloudpilot-emu/releases).
2. Generate a certificate chain and install the root certificate on the device
   running CloudpilotEmu
3. Start the server
4. Enable networking in CloudpilotEmu and point it to the proxy server.

## Download the server

The server is a small CLI application written in Python, and standalone
packages can be downloaded from the [releases
page](https://github.com/cloudpilot-emu/cloudpilot-emu/releases) on Github.
After decompressing the package archive you'll end up with a directory called
`cloudpilot-server-<version>`. This directory contains an executable
version of the server called `cloudpilot-server`, together with all
support files that are required to run it. Executables are provided for Linux
Intel/64bit, MacOS Intel/64bit and Windows Intel/64bit.

On MacOS an additional step is necessary to move the downloaded files out
of quarantine before the first run; please check the release notes for details.

If you need to run the server on another platform or use the latest version in
git, please follow the instructions under "advanced usage" below.

## Generate a certificate chain

In order to open an encrypted connection to CloudpilotEmu the server needs to
present a certificate to the browser. This certificate needs to be signed by
a trusted root certificate.

both certificates can be generated directly by `cloudpilot-server` by running

```
    > path/to/server/cloudpilot-server generate-cert
```

Note that the certificate is checked by the browser against the host or IP on
which the server runs, so `cloudpilot-server` will ask for a list of IPs and
hostnames to encode in the certificate. One of these must match with the
host or IP that you enter in CloudpilotEmu.

`cloudpilot-server` will generate a file with the suffix `.pem` and one with the
suffix `.cer`. The file with the suffix `.pem` is the certificate and key that are used
by the server. The file with the suffix `.cer` is the root certificate which needs
to be installed on the devices that access the proxy.

`cloudpilot-server` will discard the private key associated with the root certificate
immediately aftet signing the leaf certificate. Without the key, no other certificates
can be signed with the root, so the root certificate is safe to install.

## Install the certificate

Depending on the device that runs CloudpilotEmu, different steps need to be
taken to install the root certificate.

**IMPORTANT:** Always use the `...-root.cer` file for installation.

### iOS

First, copy the root certificate to your iCloud drive (or send it via mail). Tap
to open it and confirm that you want to prepare it for installation.

Now head over to `Settings` -> `General` -> `Profiles` and confirm that you want
to install the certificate. Finally, go to `Settings` -> `General` -> `About` ->
`Certificate Trust Settings` and confirm that you want to trust the certificate.

### Android

Copy the root certificate to your device (i.e. via USB or SD), navigate to
`Settings` -> `Security` -> `Credential Storage` -> `Install from device
storage` and install the certificate. When prompted select that you want to use
the certificate for "VPN and apps". Note that the exact click path to the
relevant setting may vary slightly depending on your particular Android version
and vendor.

If your device refuses to accept the certificate for "VPN and apps" and offers
"CA certificate", try that instead.

### Windows

Double click the root certificate to import it. A wizard will open that guides
you through the installation of the certificate. When asked select "Place all
certificates in the following store" (instead of automatic selection) and select
"Trusted Root Certification Authorities". Complete the wizard to install the
certificate.

If you accidentally install the certificate to the wrong store (or want to
remove it) launch "certmgr.msc" from the execute dialog in order to manage your
installed certificates.

### MacOS

Double click the root certificate in finder in order to install it. Now open
"Keychain Access" (a preinstalled app on your Mac) and locate the installed
certificate by the name that you entered when you created it. It should be in
your login keychain under "certificates".

Double click it, expand the "Trust" section and set the trust level to "Always
trust".

### Chrome for Linux

On Linux, Chrome manages its own certificate store. In order to import the
generated root certificate you need to open the settings and navigate to
"Privacy and Security" -> "Security" -> "Manage Certificates" -> "Authorities"
and click "Import". You need to set the filter to "all files" in order to see
and import the .cer file. In

### Firefox

Firefox manages its own certificate store. In order to import the root
certificate, go to the settings and navigate to "Security" -> "Certificates" ->
"Show Certificates". Go to the "Certificate Authorities" tab, select "Import"
and confirm that the certificate may be used for accessing web sites.

## Start the server

Launch the server with the generated certificate by running

```
    > path/to/server/cloudpilot-server serve --cert <certificate.pem>
```

replacing `<certificate.pem>` with the generated `.pem` file.

## Configure CloudpilotEmu

Load CloudpilotEmu, go to the settings page and enable "Network redirection". Enter the
IP address or hostname of the machine that runs the server in the field "proxy server".
You can test the connection by pressing the "Test" button on the right.

You can now use the network on your virtual PalmOS devices. Back in the days, network connectivity
was provided by connecting to a provider via modem, and PalmOS will ask you which service to
connect to if an application uses the network. You can select any service you like, CloudpilotEmu
patches PalmOS to skip dialing and directly connect via the proxy server instead.

# Setting up Network Hotsync

## Hotsync user name

Normally, CloudpilotEmu will take control of the hotsync user name used by
its virtual devices. This will conflict with Palm Desktop if you decide
to hotsync the virtual device. Go to the sessions page in CloudpilotEmu,
open the settings for the corresponding session, uncheck "Manage hotsync name"
and hit save.

## Palm Desktop

On the machine running Palm Desktop, open the Hotsync manager, go to "Connections" and
make sure that "Network (TCP/IP)" is checked.

## Palm M1xx devices

Palm decided to strip network hotsync from the M1xx line of devices, so it is not
available there.

## Palm OS

On the virtual device, open Hotsync, select "Modem Sync Prefs" in the menu and switch
to "Network" (instead of "Direct to modem"). Select "Primary PC Setup" from the menu
and enter the IP of the server running the manager as "Primary PC Address". Back on
the Hotsync main screen select "Modem" instead of "Local". You will have to select
a service --- any service will work.

You can now start syncing by tapping the Hotsync symbol.

Note that the connection to the IP that you configure in the hotsync settings
will be established from the machine running the server, so you can use
`127.0.0.1` if Hotsync is running on the same machine.

# Troubleshooting

## Can't connect to proxy

In order to troubleshoot issues with the connection to the proxy, navigate
to the proxy server directly in a browser on the device running CloudpilotEmu. In its default configuration
the server runs on port 8667, so the corresponding URL is `https://<host>:8667` with `<host>`
replaced with the hostname or IP of the machine running the server.

### Connectivity problems

If the server is reachable and the certificate is configured correctly you should see the
server respond with "not found".

If the browser complains that it can't load the page you should check whether the server
is running, whether both devices are on the same network and whether a firewall is
preventing communication between the devices.

### Certificate issues

A security warning indicates that the root certificate has not been set up correctly.

Possible issues are:

-   **Installation:** The root certificate is not installed correctly. Please check the
    instructions above.
-   **Wrong certificate:** Each generated certificate is unique, and the certificate
    installed on the device must be the exact same that `cloudpilot-server` generated
    for signing the leaf certificate used by the server.
-   **Name mismatch:** The hostname or IP configured in CloudpilotEmu must match one
    of the names or IPs that were entered when the certificate was generated.
-   **Expired:** The certificate has a lifetime of one year, and you'll have to generate
    a new one after it has expired.

If all else fails you can set an exception in most browser
in a similar way as described for firefox above, but your mileage may vary depending on
the specific browser and OS.

## Hotsync issues

Possible pitfalls to watch out for:

-   **Hotsync Manager:** The hotsync manager must be configured to enable network
    connections.
-   **Connectivity:** Make sure the primary PC address configured in PalmOS is correct.
    Note that the actual connection originates from the computer that runs the proxy server.
-   **CloudpilotEmu suspended:** CloudpilotEmu suspends execution if the browser tab (or mobile app)
    is not visible. This can be deactivated in the settings.
-   **Unsupported PalmOS version:** Palm in their infinite wisdom decided to remove network hotsync
    from their m1xx line of devices. You can work around this by installing the hotsync app
    from a backup of a non-m1xx device with a matching ROM version (you need to save a backup
    that includes ROM databases). The installed app will shadow the ROM version and work as expected.
    Network hotsync is not supported in PalmOS 2 and 1.

# Known issues

Wontfix:

-   ICMP will only work for ICMP/ECHO and requires superuser / root permissions on
    the machine running the proxy.

# Security considerations

The proxy server works by opening sockets on behalf of the client that connects and
tunnels the ensuing traffic over the websocket connection. Thus, any client
that can connect to the proxy server can use it to open sockets on the machine running
the server. **RUN THE PROXY ON YOUR LOCAL NETWORK ONLY, DO NOT EXPOSE IT TO THE
INTERNET!**

In its default configuration only https://cloudpilot-emu.github.io can connect to
the server, so drive-by attacks from other webpages are impossible.

# Advanced usage

## Running the server from git

In case you want to use the latest version of the server or want to run the server on
a system that has not prebuilt executable you'll have to run it from git. To this end
you need `python` >= 3.7 and `pipenv` installed.

Check out the CloudpilotEmu repository (or grab and extract a copy of the source code from
the releases page). Change to the `server` directory and install the dependencies with
`pipenv` running

```
    > pipenv install
```

After installing the dependencies you can run the server by running

```
    > pipenv run cloudpilot-server
```

## Running the server behind a reverse proxy

The server runs fine behind a reverse proxy. The server has two endpoints,
`/network-proxy/handshake` and `/network-proxy/connect`. which need to be proxied.
However, CloudpilotEmu supports entering a full URL instead of just a hostname
or IP in its configuration, so your proxy can prefix those endpoints with a
custom path.

In case your reverse proxy does SSL offloading you can run the server without SSL
by replacing `--cert <certificate.pem>` with `--insecure`. By default, the insecure
server runs on port 8666, and the secure server runs on 8667, but you can change
those on the command line.

## Running Cloudpilot locally or on another domain

### CORS setup

If you run CloudpilotEmu locally (during development) or on another domain the default
server configuration will not work. The reason is that the default configuration
forbids access from any origin other `https://cloudpilot-emu.github.io` via CORS.

In case you want to use networking with such a setup you can use the `--trusted-origins`
parameter to tune the CORS configuration to your needs. As an example, in order to
allow CloudpilotEmu running on `http://localhost:4600` (which is the default for the
angular dev server), invoke the server with `--trusted-origins http://localhost:4600`.
Note that you can allow more than one trusted origin by specifying multiple
origins separated by commas.

### Default protocol

CloudpilotEmu allows you to simplify the configuration by filling only the hostname
of the proxy server on the settings page. This works by determining the protocol
(https or http) from the URL on which CloudpilotEmu runs and choosing the port (8667
or 8666) accordingly.

If CloudpilotEmu runs locally this will default to http and port 8666. If you want to
connect to a proxy using SSL you'll have to enter the fully qualified URL.

## Improving security

### Limiting outgoing connections to a particular interface

A simple but effective way to reduce the impact from an attacker abusing the proxy
is to use the `--force-bind` option to limit the proxy's network access.

Passing the assigned IP address of a network interface to `--force-bind` will
force all outgoing connections to be bound to this interface, effectively
limiting them to the network segment accessible to this interface.

### Basic auth

The server can be told to do enforce basic auth by adding `--basic-auth username:password`
to the command line. The app will query the credentials when it encounters
a proxy server secured with basic auth and stores the credentials for
later use.

As usual, basic auth transmits the credentials as plain text. DON'T ENABLE IT
IF SSL IS DISABLED (`--insecure`).
