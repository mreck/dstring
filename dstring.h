// DString - A dynamic, header-only string utility library
// DStrings are automatically null-terminated.

#ifndef DSTRING_H_
#define DSTRING_H_

#define DSTR_OK    0
#define DSTR_ERROR 1

typedef struct {
    char *str;
    int length;
    int capacity;
} DString;

int  dstr_init(DString *dstr, int capacity);
int  dstr_resize(DString *dstr, int capacity);
void dstr_free(DString *dstr);
void dstr_clear(DString *dstr);
int  dstr_append(DString *dstr, char *str, int len);
void dstr_trim(DString *dstr);
void dstr_trim_left(DString *dstr);
void dstr_trim_right(DString *dstr);

#endif

#ifdef DSTRING_IMPLEMENTATION

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifndef DSTR_REALLOC
#define DSTR_REALLOC realloc
#endif

#ifndef DSTR_FREE
#define DSTR_FREE free
#endif

int dstr_init(DString *dstr, int capacity)
{
    dstr->str = NULL;
    dstr->length = 0;
    int retval = dstr_resize(dstr, capacity);
    if (retval != DSTR_OK) return retval;
    dstr->str[0] = '\0';
    return DSTR_OK;
}

int dstr_resize(DString *dstr, int capacity)
{
    char *p = (char*)DSTR_REALLOC(dstr->str, sizeof(char) * (capacity + 1));
    if (p == NULL) return DSTR_ERROR;
    dstr->str = p;
    dstr->capacity = capacity;
    if (dstr->length > dstr->capacity) {
        dstr->length = dstr->capacity;
        dstr->str[dstr->length] = '\0';
    }
    return DSTR_OK;
}

void dstr_free(DString *dstr)
{
    if (dstr->str != NULL) {
        DSTR_FREE(dstr->str);
    }
    dstr->str = NULL;
    dstr->length = 0;
    dstr->capacity = 0;
}

void dstr_clear(DString *dstr)
{
    if (dstr->str != NULL) dstr->str[0] = '\0';
    dstr->length = 0;
}

int dstr_append(DString *dstr, char *str, int len)
{
    int required = dstr->length + len;
    if (dstr->capacity < required) {
        int retval = dstr_resize(dstr, required);
        if (retval != DSTR_OK) return retval;
    }
    memcpy(dstr->str + dstr->length, str, len);
    dstr->length += len;
    dstr->str[dstr->length] = '\0';
    return DSTR_OK;
}

void dstr_trim(DString *dstr)
{
    dstr_trim_right(dstr);
    dstr_trim_left(dstr);
}

void dstr_trim_left(DString *dstr)
{
    int i = 0;
    while (i < dstr->length && isspace(dstr->str[i])) ++i;
    if (i > 0) {
        memmove(dstr->str, dstr->str + i, dstr->length - i);
        dstr->length -= i;
        dstr->str[dstr->length] = '\0';
    }
}

void dstr_trim_right(DString *dstr)
{
    while (dstr->length > 0 && isspace(dstr->str[dstr->length-1])) --dstr->length;
    dstr->str[dstr->length] = '\0';
}

#endif
