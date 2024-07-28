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

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "serial.h"

#define MAGIC_COUNT 5

bool
send_magic(serial_t fd,
           size_t   count)
{
    for (size_t i = 0; i < count; i++)
    {
        printf ("Sending magic sequence (%zu of %zu)\n", i + 1, count);

        bool rts = (i % 2);
        bool dtr = (i + 1) % 2;

        if (!serial_set(fd, rts, dtr))
        {
            /* Last one may already fail (at least on Windows) */
            return (i == count - 1);
        }

        serial_wait(10);
    }

    /* The additional set should always fail */
    return !serial_set(fd, false, false);
}

int
main(int   argc,
     char* argv[])
{
    if (argc != 2)
    {
         printf("Usage: tef-bootloader port\n");
         return -1;
    }

    serial_t fd = serial_open(argv[1]);
    if (!serial_valid(fd))
    {
        printf("Failed to open serial port: %s\n", argv[1]);
        return -1;
    }

    printf("Switching tuner (%s) into bootloader mode...\n", argv[1]);
    if (send_magic(fd, MAGIC_COUNT))
    {
        printf("Tuner has been sucessfully switched into bootloader mode\n");
        serial_close(fd);
        return 0;
    }

    printf("Failed to switch the tuner into bootloader mode\n");
    serial_close(fd);
    return -1;
}
