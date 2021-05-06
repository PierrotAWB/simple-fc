CC = gcc

# Janky way to do debugging.
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS = -g -Wall -Wextra
else
	CFLAGS = -Wall -Wextra
endif

objects = main.o cmd.o

all: $(objects)
	$(CC) $(CFLAGS) -o simplefc $(objects)

%.o: %.c defs.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f simplefc $(objects)

.PHONY : clean
