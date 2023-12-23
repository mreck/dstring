// DString - A dynamic, header-only string utility library
// DStrings are automatically null-terminated.

#ifndef DSTRING_H_
#define DSTRING_H_

#include <stdbool.h>

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
int  dstr_append_printf(DString *dstr, char *fmt, ...);
void dstr_trim(DString *dstr);
void dstr_trim_left(DString *dstr);
void dstr_trim_right(DString *dstr);
int  dstr_index_of(DString dstr, char c);
void dstr_replace(DString *dstr, char c, char replacement);
void dstr_remove(DString *dstr, char c);
bool dstr_contains(DString *dstr, char* str, int len);
bool dstr_icontains(DString *dstr, char* str, int len);

int   dstr_path_append(DString *dstr, char *str, int len);
int   dstr_path_append_printf(DString *dstr, char *fmt, ...);
char *dstr_path_filename_get(DString dstr);
int   dstr_path_filename_set(DString *dstr, char *str, int len);
char *dstr_path_ext_get(DString dstr);
int   dstr_path_ext_set(DString *dstr, char *str, int len);

#endif

#ifdef DSTRING_IMPLEMENTATION

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#ifndef DSTR_BUFFER_SIZE
#define DSTR_BUFFER_SIZE 4096
#endif

#ifndef DSTR_PATH_SEP
#ifdef _WIN32
#define DSTR_PATH_SEP '\\'
#else
#define DSTR_PATH_SEP '/'
#endif
#endif

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

int  dstr_append_printf(DString *dstr, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buff[DSTR_BUFFER_SIZE];
    int n = vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);
    return dstr_append(dstr, buff, n);
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

int dstr_index_of(DString dstr, char c)
{
    for (int i = 0; i < dstr.length; i++) {
        if (dstr.str[i] == c) {
            return i;
        }
    }
    return -1;
}

void dstr_replace(DString *dstr, char c, char replacement)
{
    for (int i = 0; i < dstr->length; i++) {
        if (dstr->str[i] == c) {
            dstr->str[i] = replacement;
        }
    }
}

void dstr_remove(DString *dstr, char c)
{
    int j = 0;
    for (int i = 0; i < dstr->length; i++) {
        if (dstr->str[i] != c) {
            dstr->str[j++] = dstr->str[i];
        }
    }
    dstr->length = j;
    dstr->str[dstr->length] = '\0';
}

bool dstr_contains(DString *dstr, char* str, int len)
{
    int d = dstr->length - len;
    for (int i = 0; i <= d; i++) {
        if (strncmp(dstr->str + i, str, len) == 0) return true;
    }
    return false;
}

bool dstr_icontains(DString *dstr, char* str, int len)
{
    int d = dstr->length - len;
    for (int i = 0; i <= d; i++) {
        if (strncasecmp(dstr->str + i, str, len) == 0) return true;
    }
    return false;
}

int dstr_path_append(DString *dstr, char *str, int len)
{
    if (dstr->str[dstr->length-1] != DSTR_PATH_SEP) {
        char buff[1] = { DSTR_PATH_SEP };
        int retval = dstr_append(dstr, buff, 1);
        if (retval != DSTR_OK) return retval;
    }
    return dstr_append(dstr, str, len);
}

int dstr_path_append_printf(DString *dstr, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buff[DSTR_BUFFER_SIZE];
    int n = vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);
    return dstr_path_append(dstr, buff, n);
}

char *dstr_path_filename_get(DString dstr)
{
    char *end = dstr.str + dstr.length;
    char *p = end - 1;
    while (p >= dstr.str && *p != DSTR_PATH_SEP) --p;
    ++p;
    return p < end ? p : NULL;
}

int dstr_path_filename_set(DString *dstr, char *str, int len)
{
    char *p = dstr_path_filename_get(*dstr);
    if (p != NULL) {
        dstr->length = p - dstr->str;
    }
    return dstr_append(dstr, str, len);
}

char *dstr_path_ext_get(DString dstr)
{
    char *p = dstr.str + (dstr.length - 1);
    while (p > dstr.str && *p != DSTR_PATH_SEP && *p != '.') --p;
    return *p == '.' ? p : NULL;
}

int dstr_path_ext_set(DString *dstr, char *str, int len)
{
    char *p = dstr_path_ext_get(*dstr);
    if (p != NULL) {
        dstr->length = p - dstr->str;
    }
    if (len > 0 && *str != '.') {
        int retval = dstr_append(dstr, ".", 1);
        if (retval != DSTR_OK) return retval;
    }
    return dstr_append(dstr, str, len);
}

#endif
