CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -O2 -march=nehalem -I.
LFLAGS =
OBJS=src/bv.o src/field.o src/grass.o

all: grass asgn8.pdf

grass: $(OBJS)
	$(CC) $(LDFLAGS) -o grass $(OBJS)

asgn8.pdf: asgn8.tex
	pdflatex asgn8.tex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tidy:
	$(RM) $(OBJS) asgn8.aux asgn8.listing asgn8.log asgn8.out

clean: tidy
	$(RM) grass asgn8.pdf

# --use-cc=clang allows it to run without GCC installed per @242 on Piazza
scan-build: clean
	scan-build --use-cc=clang make

# suggested by tutor Eric
format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean scan-build format tidy
