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

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

typedef HANDLE serial_t;

serial_t
serial_open(const char* port)
{
    char name[256];
    snprintf(name, sizeof(name), "\\\\.\\%s", port);

    serial_t fd = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    return fd;
}

bool
serial_valid(serial_t fd)
{
    return fd != INVALID_HANDLE_VALUE;
}

void
serial_close(serial_t fd)
{
    CloseHandle(fd);
}

bool
serial_set(serial_t fd,
           bool     rts,
           bool     dtr)
{
    DCB dcbSerialParams = {0};
    if (!GetCommState(fd, &dcbSerialParams))
    {
        return false;
    }

    dcbSerialParams.fRtsControl = rts ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
    dcbSerialParams.fDtrControl = dtr ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;

    return SetCommState(fd, &dcbSerialParams);
}

void
serial_wait(size_t length)
{
    Sleep(length);
}
