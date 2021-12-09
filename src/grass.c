#include "field.h"

#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTIONS "aswrvhi:n:S:"

typedef void (*toucher_t)(Field *, uint32_t, unsigned int);
typedef struct {
    char *name;
    int flag;
    toucher_t func;
} ToucherInfo;

#define SEQUENTIAL 1
#define WIDE       2
#define RANDOM     4

#define N_TOUCHERS 3
ToucherInfo touchers[] = {
    { "Sequential", SEQUENTIAL, field_touch_sequential },
    { "Wide", WIDE, field_touch_wide },
    { "Random", RANDOM, field_touch_random },
};

#define DEFAULT_SEED 7566707
#define DEFAULT_SIZE 10
#define MAX_SIZE 1024

void print_usage(char *self) {
    printf("SYNOPSIS\n"
           "  Compare different methods of touching grass.\n"
           "\n"
           "USAGE\n"
           "  %s [-aswrvh] [-f size] [-n seed]\n"
           "\n"
           "OPTIONS\n"
           "  -a           Test all methods.\n"
           "  -s           Test the sequential method.\n"
           "  -w           Test the wide method.\n"
           "  -r           Test the random method.\n"
           "  -v           Show verbose statistics.\n"
           "  -h           Print this help information.\n"
           "  -i iters     Set the number of iterations to run (default: size^2).\n"
           "  -n size      Set the size of the field of grass (1-%d, default: %d).\n"
           "  -S seed      Set the random seed for -r (default: %d).\n",
        self, MAX_SIZE, DEFAULT_SIZE, DEFAULT_SEED);
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t touchers_selected = 0;
    bool verbose = false, overrode_iters = false;
    uint32_t size = DEFAULT_SIZE, iters = DEFAULT_SIZE * DEFAULT_SIZE;
    unsigned int seed = DEFAULT_SEED;

    if (argc == 1) {
        print_usage(argv[0]);
        return 1;
    }

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': touchers_selected = SEQUENTIAL | WIDE | RANDOM; break;
        case 's': touchers_selected |= SEQUENTIAL; break;
        case 'w': touchers_selected |= WIDE; break;
        case 'r': touchers_selected |= RANDOM; break;
        case 'v': verbose = true; break;
        case 'h': print_usage(argv[0]); return 1;
        case 'n':
            size = strtoul(optarg, NULL, 10);
            if (size < 1 || size > MAX_SIZE) {
                fprintf(
                    stderr, "%s: invalid size %" PRIu32 ". valid sizes are 1-%d.\n", argv[0], size, MAX_SIZE);
                return 1;
            }
            if (!overrode_iters) {
                iters = size * size;
            }
            break;
        case 'i':
            iters = strtoul(optarg, NULL, 10);
            overrode_iters = true;
            break;
        case 'S': seed = strtoul(optarg, NULL, 10); break;
        default: print_usage(argv[0]); return 1;
        }
    }

    if (touchers_selected == 0) {
        fprintf(stderr,
            "%s: no method(s) selected. select one with -a, -s, -w, or -r. -h for help.\n",
            argv[0]);
        return 1;
    }

    for (uint32_t i = 0; i < N_TOUCHERS; i += 1) {
        ToucherInfo ti = touchers[i];
        if (touchers_selected & ti.flag) {
            Field *f = field_create(size);
            ti.func(f, iters, seed);
            printf("%s:\n", ti.name);
            if (verbose) {
                uint32_t count = field_count(f), writes = field_writes(f);
                double efficiency = 100.0 * count / writes;
                printf("  Touched blades:   %5" PRIu32 "\n", field_count(f));
                printf("  BitVector writes: %5" PRIu32 "\n", field_writes(f));
                printf("  Efficiency:       %8.2f%%\n", efficiency);
            } else {
                field_print(f);
            }
            field_delete(&f);
        }
    }

    return 0;
}
