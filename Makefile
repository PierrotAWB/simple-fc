CC = gcc

# Janky way to do debugging.
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS = -g -Wall -Wextra
else
	CFLAGS = -Wall -Wextra
endif

objects = main.o cmd.o

simplefc: $(objects)
	$(CC) $(CFLAGS) -o simplefc $(objects)

%.o: %.c defs.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f simplefc $(objects)

install: simplefc
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f simplefc $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/simplefc

.PHONY : clean
