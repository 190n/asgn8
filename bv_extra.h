#pragma once

#include "bv.h"

uint32_t bv_count(BitVector *bv);

bool bv_get_64(BitVector *bv, uint32_t i, uint64_t *word);
