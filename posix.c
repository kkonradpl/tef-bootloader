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

#include <stdbool.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

typedef int serial_t;

serial_t
serial_open(const char* port)
{
    return open(port, O_RDWR | O_NOCTTY);
}

bool
serial_valid(serial_t fd)
{
    return fd >= 0;
}

void
serial_close(serial_t fd)
{
    close(fd);
}

bool
serial_set(serial_t fd,
           bool     rts,
           bool     dtr)
{
    int ctl;

    if (ioctl(fd, TIOCMGET, &ctl) < 0)
    {
        return false;
    }

    if (rts)
    {
        ctl |= TIOCM_RTS;
    }
    else
    {
        ctl &= ~TIOCM_RTS;
    }

    if (dtr)
    {
        ctl |= TIOCM_DTR;
    }
    else
    {
        ctl &= ~TIOCM_DTR;
    }

    return ioctl(fd, TIOCMSET, &ctl) >= 0;
}

void
serial_wait(size_t length)
{
    usleep(length * 1000);
}
