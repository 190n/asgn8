#include "bv.h"
#include "bv_extra.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>

// Copied from assignment document
struct BitVector {
    uint32_t length;
    uint64_t *vector;
    uint32_t writes;
};

#define BITS_PER_UNIT 64

//
// Given the index of a bit, calculate which unit in a BitVector contains it.
//
// i: the bit index
//
#define BV_UNIT(i) ((i) / BITS_PER_UNIT)

//
// Given the index of a bit, calculate which bit it is in a unit (0-63).
//
// i: the bit index
//
#define BV_BIT(i) ((i) % BITS_PER_UNIT)

//
// Given a length in bits, calculate the number of units required to store that many bits.
//
// length: the length in bits
//
static inline uint32_t bv_length_in_units(uint32_t length) {
    return length % BITS_PER_UNIT ? (length / BITS_PER_UNIT + 1) : (length / BITS_PER_UNIT);
}

//
// Allocate a new BitVector.
//
// length: length in bits of the BitVector
//
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    // only set fields on v if malloc succeeded
    if (bv != NULL) {
        bv->length = length;
        bv->writes = 0;
        // calculate number of bytes needed
        uint32_t units = bv_length_in_units(length);
        // calloc initializes bytes to zero
        bv->vector = (uint64_t *) calloc(units, sizeof(uint64_t));
        // make sure that allocation succeeded too
        if (bv->vector == NULL) {
            free(bv);
            bv = NULL; // so we will return NULL
        }
    }
    return bv;
}

//
// Free all memory used by a BitVector, and set the pointer to NULL.
//
// bv: pointer to a pointer to the BitVector to delete
//
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

//
// Get the length of a BitVector.
//
// bv: pointer to the BitVector to inspect
//
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

//
// Set a bit in a BitVector to 1.
//
// bv: pointer to the BitVector to modify
// i:  index of the bit to set
//
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i < bv->length) {
        uint64_t mask = (uint64_t) 1 << BV_BIT(i);
        bv->vector[BV_UNIT(i)] |= mask;
        bv->writes += 1;
        return true;
    } else {
        return false;
    }
}

//
// Clear a bit in a BitVector (set it to 0).
//
// bv: pointer to the BitVector to modify
// i:  index of the bit to clear
//
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i < bv->length) {
        uint64_t mask = ~((uint64_t) 1 << BV_BIT(i));
        bv->vector[BV_UNIT(i)] &= mask;
        bv->writes += 1;
        return true;
    } else {
        return false;
    }
}

//
// Get a bit in a BitVector.
//
// bv: pointer to the BitVector to look in
// i:  index of the bit to retrieve
//
bool bv_get_bit(BitVector *bv, uint32_t i, bool *bit) {
    if (i < bv->length) {
        uint64_t unit = bv->vector[BV_UNIT(i)];
        *bit = (unit >> BV_BIT(i)) & 0x01;
        return true;
    } else {
        return false;
    }
}

//
// Set the 64-bit unit in a BitVector containing a given index to all 1.
//
// bv: pointer to the BitVector to modify
// i:  index around which to set bits
//
bool bv_set_64(BitVector *bv, uint32_t i) {
    if (i < bv->length) {
        bv->vector[BV_UNIT(i)] = ~0;
        bv->writes += 1;
        return true;
    } else {
        return false;
    }
}

//
// Clear the 64-bit unit in a BitVector containing a given index (set it to all 0).
//
// bv: pointer to the BitVector to modify
// i:  index around which to clear bits
//
bool bv_clr_64(BitVector *bv, uint32_t i) {
    if (i < bv->length) {
        bv->vector[BV_UNIT(i)] = 0;
        bv->writes += 1;
        return true;
    } else {
        return false;
    }
}

//
// Get the number of writes that have been made to a BitVector.
//
// bv: pointer to the BitVector to inspect
//
uint32_t bv_writes(BitVector *bv) {
    return bv->writes;
}

//
// Print the bits in a BitVector.
//
// bv: pointer to the BitVector to print
//
void bv_print(BitVector *bv) {
    fprintf(stderr, "[bv_print] bv with length %" PRIu32 ": ", bv->length);
    bool bit;
    for (uint32_t i = 0; i < bv->length; i += 1) {
        bv_get_bit(bv, i, &bit);
        fputc(bit ? '1' : '0', stderr);
    }
    fprintf(stderr, "\n");
}

//
// Count the set bits in a BitVector.
//
// bv: pointer to the BitVector
//
uint32_t bv_count(BitVector *bv) {
    uint32_t count = 0, len_units = bv_length_in_units(bv->length);
    for (uint32_t i = 0; i < len_units; i += 1) {
        uint64_t unit = bv->vector[i];
        if (i == len_units - 1 && bv->length % 64 != 0) {
            // zero any extraneous bits (maybe set with bv_set_64)
            uint64_t mask = ~0ul >> (64 - (bv->length % 64));
            unit &= mask;
        }

        // adapted from code by Matt Godbolt
        // https://youtu.be/bSkpMdDe4g4?t=2387
        while (unit) {
            count += 1;
            unit &= (unit - 1);
        }
    }

    return count;
}
