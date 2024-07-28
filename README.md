tef-bootloader
=======

Application that allows to switch TEF Headless USB Tuner into the bootloader mode

Copyright (C) 2024  Konrad Kosmatka

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

# Build
In order to build  you will need:

- C compiler
- make

Once you have all the necessary dependencies, you can use:

- GNU/Linux (POSIX): `make`
- Windows: `make windows`

# Usage

```sh
$ tef-bootloader /dev/ttyACM0
```

Expected output:
```
Switching tuner (/dev/ttyACM0) into bootloader mode...
Sending magic sequence (1 of 5)
Sending magic sequence (2 of 5)
Sending magic sequence (3 of 5)
Sending magic sequence (4 of 5)
Sending magic sequence (5 of 5)
Tuner has been sucessfully switched into bootloader mode
```