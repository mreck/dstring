#include <assert.h>
#include <stdio.h>

#define DSTRING_IMPLEMENTATION
#include "dstring.h"

#define FOO "foo"
#define BAR "bar"
#define BAZ "baz"
#define SPACE "                                    "

int main(void)
{
    DString s;

    assert(dstr_init(&s, 10) == DSTR_OK);
    assert(dstr_append(&s, FOO, 3) == DSTR_OK);
    assert(dstr_append(&s, BAR, 3) == DSTR_OK);
    assert(dstr_append(&s, BAZ, 3) == DSTR_OK);
    printf("s = |%s|\n", s.str);

    assert(dstr_append(&s, FOO, 3) == DSTR_OK);
    assert(dstr_append(&s, BAR, 3) == DSTR_OK);
    assert(dstr_append(&s, BAZ, 3) == DSTR_OK);
    printf("s = |%s|\n", s.str);

    assert(dstr_resize(&s, 12) == DSTR_OK);
    printf("s = |%s|\n", s.str);

    assert(dstr_resize(&s, 24) == DSTR_OK);
    printf("s = |%s|\n", s.str);

    dstr_clear(&s);
    printf("s = |%s|\n", s.str);

    assert(dstr_append(&s, SPACE, 4) == DSTR_OK);
    assert(dstr_append(&s, FOO, 3) == DSTR_OK);
    assert(dstr_append(&s, SPACE, 4) == DSTR_OK);
    printf("s = |%s|\n", s.str);

    dstr_trim(&s);
    printf("s = |%s|\n", s.str);

    assert(dstr_append(&s, BAR, 3) == DSTR_OK);
    assert(dstr_append(&s, BAZ, 3) == DSTR_OK);
    dstr_replace(&s, 'a', 'o');
    printf("s = |%s|\n", s.str);

    dstr_remove(&s, 'o');
    printf("s = |%s|\n", s.str);

    dstr_clear(&s);
    assert(dstr_append_printf(&s, "%.*s %f", 2, SPACE, 3.14f) == DSTR_OK);
    printf("s = |%s|\n", s.str);

    dstr_free(&s);
    dstr_free(&s);

    return 0;
}
