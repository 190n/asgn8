CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -O2 -g
LFLAGS =
OBJS=grass.o bv.o

all: grass

grass: $(OBJS)
	$(CC) $(LDFLAGS) -o grass $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f grass $(OBJS)

# --use-cc=clang allows it to run without GCC installed per @242 on Piazza
scan-build: clean
	scan-build --use-cc=clang make

# suggested by tutor Eric
format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean scan-build format
