#include "field.h"

#include <inttypes.h>
#include <stdio.h>

int main(void) {
    Field *f = field_create(32);
    field_touch_random(f, 48, 347928);
    field_print(f);
    printf("%"PRIu32" writes\n", field_writes(f));
    field_delete(&f);
    return 0;
}
