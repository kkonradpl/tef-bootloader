CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -c -O2
INSTALL = install -c
TARGET = tef-bootloader

PREFIX = $(DESTDIR)/usr
BINDIR = $(PREFIX)/bin

tef-bootloader:	tef-bootloader.c serial.h posix.c
	$(CC) -o $(TARGET) tef-bootloader.c posix.c

.PHONY:	windows
windows:	tef-bootloader.c serial.h windows.o
	$(CC) -o $(TARGET) tef-bootloader.c windows.o

.PHONY:	clean
clean:
	rm -f tef-bootloader

.PHONY:	install
install:	tef-bootloader
	$(INSTALL) $(TARGET) $(BINDIR)/$(TARGET)

.PHONY:	uninstall
uninstall:
	rm -f $(BINDIR)/$(TARGET)
