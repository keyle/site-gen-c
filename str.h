#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct {
    char *str;
    size_t size;
} String;

String *string_create(const char *str) {
    String *s = malloc(sizeof(String));

    if (!s) {
        return NULL;
    }

    s->str = strdup(str);
    s->size = strlen(s->str);

    return s;
}

String *string_copy(const String *str) {
    String *copy = malloc(sizeof(String));
    if (!copy) {
        return NULL;
    }

    copy->str = strdup(str->str);
    copy->size = str->size;

    return copy;
}
#endif
