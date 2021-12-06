#include "field.h"

#include <stdio.h>
#include <stdlib.h>

struct Field {
    uint32_t size;
    BitVector *matrix;
};

#define MIN(x, y) ((x) < (y) ? (x) : (y))

//
// Allocate a new Field.
//
// size: length of one side of the Field
//
Field *field_create(uint32_t size) {
    Field *f = (Field *) malloc(sizeof(Field));
    if (f) {
        f->size = size;
        f->matrix = bv_create(size * size);
        if (!f->matrix) {
            free(f);
            f = NULL;
        }
    }
    return f;
}

//
// Delete a Field, and set the pointer to NULL.
//
// f: double pointer to the Field
//
void field_delete(Field **f) {
    bv_delete(&(*f)->matrix);
    free(*f);
    *f = NULL;
}

//
// Get the size of a Field.
//
// f: pointer to the Field
//
uint32_t field_size(Field *f) {
    return f->size;
}

//
// Get the area of a Field.
//
// f: pointer to the Field
//
uint32_t field_area(Field *f) {
    return bv_length(f->matrix);
}

//
// Get the number of flattened blades in a Field.
//
// f: pointer to the Field
//
uint32_t field_count(Field *f) {
    uint32_t count = 0;
    bool bit;
    for (uint32_t i = 0; i < field_area(f); i += 1) {
        bv_get_bit(f->matrix, i, &bit);
        if (bit) {
            count += 1;
        }
    }
    return count;
}

//
// Get the number of writes performed on a Field.
//
// f: pointer to the Field
//
uint32_t field_writes(Field *f) {
    return bv_writes(f->matrix);
}

//
// Touch up to max_iters blades of grass, starting from the first one.
//
// f:         pointer to the Field
// max_iters: maximum iterations to perform (no more than those necessary will be performed)
// seed:      unused
//
void field_touch_sequential(Field *f, uint32_t max_iters, unsigned int seed) {
    // no it's not unused i swear
    (void) seed;

    uint32_t end = MIN(max_iters, field_area(f));
    for (uint32_t i = 0; i < end; i += 1) {
        bv_set_bit(f->matrix, i);
    }
}

//
// Touch grass, 64 blades at a time, up to max_iters times, starting from the first one.
//
// f:         pointer to the Field
// max_iters: maximum iterations to perform (no more than those necessary will be performed)
// seed:      unused
//
void field_touch_wide(Field *f, uint32_t max_iters, unsigned int seed) {
    (void) seed;

    uint32_t area = field_area(f), necessary_iters = area % 64 == 0 ? (area / 64) : (area / 64 + 1),
             end = MIN(max_iters, necessary_iters);

    for (uint32_t i = 0; i < end; i += 1) {
        bv_set_64(f->matrix, i * 64);
    }
}

//
// Touch grass, one blade at a time, max_iters times, randomly.
//
// f:         pointer to the Field
// max_iters: number of iterations to perform
// seed:      seed for the random number generator
//
void field_touch_random(Field *f, uint32_t max_iters, unsigned int seed) {
    srandom(seed);
    for (uint32_t i = 0; i < max_iters; i += 1) {
        uint32_t blade = random() % field_area(f);
        bv_set_bit(f->matrix, blade);
    }
}

//
// Print a square Field, using / for a standing blade and _ for a flattened one.
//
// f: pointer to the Field
//
void field_print(Field *f) {
    bool bit;
    for (uint32_t row = 0; row < f->size; row += 1) {
        for (uint32_t col = 0; col < f->size; col += 1) {
            uint32_t index = row * f->size + col;
            bv_get_bit(f->matrix, index, &bit);
            putc(bit ? '_' : '/', stdout);
        }

        putc('\n', stdout);
    }
}
