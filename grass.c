#include "field.h"

#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTIONS "aswrvhf:i:n:"

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
           "  -f size      Set the size of the field of grass (1-50, default: 10).\n"
           "  -i iters     Set the number of iterations to run (default: size^2).\n"
           "  -n seed      Set the random seed for -r (default: 7566707).\n",
        self);
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t touchers_selected = 0;
    bool verbose = false, overrode_iters = false;
    uint32_t size = 10, iters = 100;
    unsigned int seed = 7566707;

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
        case 'f':
            size = strtoul(optarg, NULL, 10);
            if (size < 1 || size > 50) {
                fprintf(
                    stderr, "%s: invalid size %" PRIu32 ". valid sizes are 1-50.\n", argv[0], size);
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
        case 'n': seed = strtoul(optarg, NULL, 10); break;
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
            field_print(f);
            field_delete(&f);
        }
    }

    return 0;
}
