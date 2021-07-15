This document describes how Cloudpilot can be set up or network access in the
virtual device and configured to do Network Hotsync.

# Basic Setup

As a web application, Cloudpilot cannot access the network directly. In order
to access the network you need to run a small proxy server locally and
configure Cloudpilot to use it. As the official version of Cloudpilot is
hosted over HTTPS, the connection between the proxy server and Cloudpilot
has to be encrypted as well.

The necessary steps to set up networking are

1. Download latest server version from the
   [releases page](https://github.com/cloudpilot-emu/cloudpilot/releases).
2. Generate a certificate and install it on the device running Cloudpilot
3. Enable networking in Cloudpilot and point it to the proxy server.

## Download the server

The server is a small CLI application written in Python, and prepackaged
executables can be downloaded from the [releases
page](https://github.com/cloudpilot-emu/cloudpilot/releases) on Github. These
executables can be run directly on the Shell (or Powershell on Windows) and
don't require Python or any other setup. Executables are provided for Linux
Intel/64bit, MacOS Intel/64bit and Windows Intel/64bit.

If you need to run the server on another platform or use the latest version in
git, please follow the instructions under "advanced usage" below.

## Generate a certificate

In order to open an encrypted connection to Cloudpilot the server needs a
self-signed certificate installed on the device running Cloudpiltot.

This certificate can be generated directly by `cloudpilot-server` by doing

```
    $ cloudpilot-server generate-cert
```

Note that the certificate is checked by the browser against the host or IP on
which the server runs, so `cloudpilot-server` will ask for a list of IPs and
hostnames to encode in the certificate.

`cloudpilot-server` will generate a file with the suffix `.pem` and one with the
suffix `.cer`. The file with the suffix `.pem` contains the private key
associated with the certificate and is used by the server. DO NOT DISTRIBUTE IT
as an attacker could try to use it for mounting an attack on SSL connections
originating from devices that have the certificate installed and trusted
(altough care has been taken to make the cert as safe to use as possible).

The `.cer` file does not contain sensitive data and is safe to distribute. It
has to be installed on the device(s) running Cloudpilot.

## Install the certificate

Depending on the device that runs Cloudpilot, different steps need to be taken to
install the certificate.

IMPORTANT: Always use the `.cer` file for installation, the `.pem` should be considered
sensitive data and remain with the server.

### iOS

First, copy the certificate to your iCloud drive (or send it via mail). Double tap
to open it and confirm that you want to prepare it for installation.

Now head over to `Settings` -> `General` -> `Profiles` and confirm that you want to install
the certificate. Finalle, go to `Settings` -> `General` -> `About` -> `Certificate Trust Settings`
and confirm that you want to trust the certificate.

### Android

Copy the certificate to your device (i.e. via USB) and head to `Settings` -> `Security` ->
`Credential Storage` -> `Install from device storage`
and install the certificate. When prompted select that you want to
use the certificate for "App and VPN". Note that the exact click path to the
relevant setting may vary slightly depending on your particular Android version and vendor.
