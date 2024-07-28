CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -c -O2
INSTALL = install -c
TARGET = tef-bootloader

PREFIX = $(DESTDIR)/usr
BINDIR = $(PREFIX)/bin

tef-bootloader:	tef-bootloader.o
	$(CC) -o $(TARGET) tef-bootloader.o $(LIBS)

tef-bootloader.o: tef-bootloader.c
	$(CC) $(CFLAGS) tef-bootloader.c

.PHONY:	clean
clean:
	rm -f *.o tef-bootloader

.PHONY:	install
install:	tef-bootloader
	$(INSTALL) $(TARGET) $(BINDIR)/$(TARGET)

.PHONY:	uninstall
uninstall:
	rm -f $(BINDIR)/$(TARGET)
