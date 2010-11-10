sources = data.c exceptions.c machine.c main.c utils.c validator.c
objects = $(sources:.c=.o)
bin = turing-simulator

D_CFLAGS = -g -ggdb3 -Wall -Wextra -Werror -pedantic -std=gnu99 -O3 -march=native
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
