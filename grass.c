#include "bv.h"

#include <inttypes.h>
#include <stdio.h>

int main(void) {
    BitVector *bv = bv_create(96);
    bv_set_64(bv, 64);
    bv_set_64(bv, 0);
    bv_clr_64(bv, 32);
    bv_set_bit(bv, 5);
    bv_clr_bit(bv, 93);
    bv_print(bv);
    printf("%" PRIu32 " writes\n", bv_writes(bv));
    bv_delete(&bv);
    return 0;
}
