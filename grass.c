#include "bv.h"

int main(void) {
    BitVector *bv = bv_create(96);
    bv_set_64(bv, 64);
    bv_set_64(bv, 0);
    bv_clr_64(bv, 32);
    bv_set_bit(bv, 5);
    bv_print(bv);
    bv_delete(&bv);
    return 0;
}
