This is the CloudpilotEmu fork of Dmitry Grinberg's
[uARM](https://github.com/uARM-Palm/uARM). The codebase currently supports only
the Tungsten E2. The goal is to rework and optimize uARM to the point where it
can be integrated in CloudpilotEmu.

# Building

If necessary, copy `Makefile.local.example` to `Makefile.local` and adjust it to
your liking. You also need to install `libsdl2-dev` and `libsdl2-image-dev`.
Then do

```
    $ make -Csrc
```

to build the native binary.

# LICENSE

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
