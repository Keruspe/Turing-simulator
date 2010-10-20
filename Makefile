sources = main.c machine.c data.c utils.c exceptions.c
objects = $(sources:.c=.o)
bin = turing-simulator

D_CFLAGS = -g -ggdb -Wall -Wextra -Werror -pedantic -std=gnu99 -O2 -march=native
D_LDFLAGS = -Wl,-O2 -Wl,--as-needed

CC = gcc
RM = rm -f
ECHO = echo

CFLAGS := $(D_CFLAGS) $(CFLAGS)
LDFLAGS := $(D_LDFLAGS) $(LDFLAGS)

all: $(bin)

$(bin): $(objects)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	$(RM) $(objects) $(bin)
