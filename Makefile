CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2
RM = rm
RMFLAGS = -f

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

default: all

all: bin/motd

bin/motd: src/main.c src/motd.c src/pipe.c
	$(CC) $(CFLAGS) -o bin/motd src/main.c src/motd.c src/pipe.c

install: bin/motd
	install -m 755 bin/motd $(PREFIX)/bin

clean veryclean:
	$(RM) $(RMFLAGS) bin/motd

uninstall:
	$(RM) $(RMFLAGS) $(PREFIX)/bin/motd    

