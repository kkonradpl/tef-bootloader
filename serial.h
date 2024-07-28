/*
 *  tef-bootloader
 *  Copyright (C) 2024  Konrad Kosmatka

 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef TEF_BOOTLOADER_SERIAL_H
#define TEF_BOOTLOADER_SERIAL_H

#ifdef __WIN32__
#include <windows.h>
typedef HANDLE serial_t;
#else
typedef int serial_t;
#endif

serial_t serial_open(const char* port);
bool serial_valid(serial_t fd);
void serial_close(serial_t fd);
bool serial_set(serial_t fd, bool rts, bool dtr);
bool serial_wait(size_t length);

#endif
