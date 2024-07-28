/*
 *  tef-bootloader v1.0
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
#include <stdlib.h>
#include <windows.h>


HANDLE
serial_open(const char* port)
{
    char name[256];
    snprintf(name, sizeof(name), "\\\\.\\%s", port);

    HANDLE fd = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (fd == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open serial port: %s\n", port);
        exit(EXIT_FAILURE);
    }

    return fd;
}

void
serial_magic(HANDLE fd, 
             int    count)
{
    DCB dcbSerialParams = {0};
    if (!GetCommState(fd, &dcbSerialParams))
    {
        printf("Failed to get serial port parameters\n");
        CloseHandle(fd);
        exit(EXIT_FAILURE);
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    for (int i = 0; i < count; i++)
    {
        dcbSerialParams.fRtsControl = (i % 2) ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
        dcbSerialParams.fDtrControl = (i + 1) % 2 ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;

        if (!SetCommState(fd, &dcbSerialParams))
        {
            if (i == count - 1)
            {
                printf("Tuner has been sucessfully switched into bootloader mode\n");
                exit(EXIT_SUCCESS);
            }

            printf("Failed to set serial port parameters\n");
            CloseHandle(fd);
            exit(EXIT_FAILURE);
        }

        Sleep(10);
    }
}

int
main(int   argc,
     char* argv[])
{
    if (argc != 2)
    {
         printf("No serial port chosen\n");
         return -1;
    }

    HANDLE fd = serial_open(argv[1]);

    printf("Switching tuner (%s) into bootloader mode...\n", argv[1]);
    serial_magic(fd, 5);

    printf("Failed to switch the tuner into bootloader mode\n");
    CloseHandle(fd);
    return -1;
}
