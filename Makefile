CC = gcc
LD = ld
CFLAGS = -Wall -Werror -lalpm -O3
RM = rm
RMFLAGS = -f

SOURCEDIR = src
HEADERDIR = lib
BINDIR = bin
TARGET = $(BINDIR)/motd
SRC = $(wildcard $(SOURCEDIR)/*.c)

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

.PHONY: all clean

default: all

all: $(TARGET)

$(TARGET): $(SRC) | $(BINDIR)
	$(CC) $(CFLAGS) -I$(HEADERDIR) -o $(TARGET) -g $(SRC)

$(BINDIR):
	mkdir -p $@

install: $(TARGET)
	install -m 755 $(TARGET) $(PREFIX)/$(BINDIR)

clean veryclean:
	$(RM) $(RMFLAGS) $(TARGET)

uninstall:
	$(RM) $(RMFLAGS) $(PREFIX)/$(TARGET) 
