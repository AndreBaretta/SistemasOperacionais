# Target binary name and installation directory
TARGET = ex2
SRC = ex2.c
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

CC = gcc
CFLAGS = -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

install: $(TARGET)
	sudo cp $(TARGET) $(BINDIR)
	rm -f $(TARGET)

uninstall:
	sudo rm -f $(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET)
