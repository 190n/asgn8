#include "field.h"

int main(void) {
    Field *f = field_create(6);
    field_touch_random(f, 4, 374);
    field_print(f);
    field_delete(&f);
    return 0;
}
