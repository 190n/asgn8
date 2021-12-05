#pragma once

#include "bv.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Field Field;

Field *field_create(uint32_t size);

void field_delete(Field **f);

uint32_t field_size(Field *f);

uint32_t field_area(Field *f);

uint32_t field_count(Field *f);

uint32_t field_writes(Field *f);

void field_touch_sequential(Field *f, uint32_t max_iters, unsigned int seed);

void field_touch_wide(Field *f, uint32_t max_iters, unsigned int seed);

void field_touch_random(Field *f, uint32_t max_iters, unsigned int seed);

void field_print(Field *f);
