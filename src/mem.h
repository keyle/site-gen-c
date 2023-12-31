#ifndef MEM_H
#define MEM_H

#include "mem_internal.h"

void* _debug_malloc(size_t size, char file[static 1], size_t line) {
    void* p = malloc(size);
    if (!p) {
        fprintf(stderr, "could not allocate!");
        exit(1);
    }
    printf("\033[36m* malloc %s:%zu (%zu bytes) at %p\033[0m\n", file, line, size, p);

    _debug_add(file, line, size, p);

    return p;
}

void* _debug_calloc(size_t count, size_t size, char file[static 1], size_t line) {
    void* p = calloc(count, size);
    printf("\033[36m* calloc %s:%zu (%zu * %zu = %zu bytes) at %p\033[0m\n", file, line, count, size, count * size, p);
    _debug_add(file, line, count * size, p);
    return p;
}

void* _debug_realloc(void* p, size_t size, char file[static 1], size_t line) {
    printf("\033[33m* realloc %s:%zu (%zu bytes) at %p\033[0m\n", file, line, size, p);
    _debug_add(file, line, size, p);
    return realloc(p, size);
}

void _debug_free(void* p, char file[static 1], size_t line) {
    printf("\033[32m* free %s:%zu %p\033[0m\n", file, line, p);
    _debug_remove(p);
    free(p);
}

#define malloc(s) _debug_malloc(s, __FILE__, __LINE__) // replace malloc with debug version
#define calloc(c, s) _debug_calloc(c, s, __FILE__, __LINE__)
#define realloc(p, s) _debug_realloc(p, s, __FILE__, __LINE__)
#define free(p) _debug_free(p, __FILE__, __LINE__)

#endif
