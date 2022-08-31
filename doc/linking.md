CloudpilotEmu can be linked from other webpages in order to automatically import
ROMs or sessions or to automatically install databases from a supplied URL.

When linking via a `<a>` tag, be sure to include a `target="cloudpilot"` attribute.
This will cause the browser to reuse a previously opened tab if another link is clicked
(instead of reloading the page over and over again).

Please note that the server hosting these files needs to support CORS. Unless
basic auth is used, setting the `Access-Control-Allow-Origin` header to `*` is
sufficient.

# Importing sessions and ROMs

An URL for importing a session or ROM file has the form

```
https://cloudpilot-emu.github.io/app/#/import?url=url_encoded_rom_url
```

The `url` query parameter contains the full URL encoded URL to the file.

Navigating to such an URL will open CloudpilotEmu, navigate to the session tab and prompt
the user to import the file from the supplied URL.

# Installing databases

An URL for installing a database (or a zip archive with databases) has the form

```
https://cloudpilot-emu.github.io/app/#/install?url=url_encoded_database_url
```

The `url` query parameter contains the full URL encoded URL to the file.

Navigating to such an URL will open CloudpilotEmu, navigate to the emulation tab and prompt
the user to install the database(s) into their currently running session.
