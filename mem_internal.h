#ifndef MEM_INTERNAL_H
#define MEM_INTERNAL_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    size_t size;
    size_t line;
    void* p;
    char* file;
} Meminfo;

static Meminfo** meminfos = NULL;
static size_t meminfo_i = 0;
static size_t meminfo_sz = 0;

void _debug_grow_if_needed() {
    if (meminfo_i < meminfo_sz) {
        return;
    }
    size_t new_size = 1 + (meminfo_sz * 3);
    Meminfo** temp = realloc(meminfos, sizeof(Meminfo*) * new_size);
    if (!temp) {
        fprintf(stderr, "Failed to reallocate memory\n");
        exit(1);
    }
    meminfos = temp;
    meminfo_sz = new_size;
}

void _debug_add(char* file, size_t line, size_t size, void* p) {
    _debug_grow_if_needed();
    Meminfo* new_meminfo = malloc(sizeof(Meminfo));
    if (new_meminfo) {
        *new_meminfo = (Meminfo){.size = size, .line = line, .p = p, .file = file};
        meminfos[meminfo_i++] = new_meminfo;
    } else {
        fprintf(stderr, "Failed to allocate Meminfo\n");
        exit(1);
    }
}

void _debug_remove(void* p) {
    for (size_t i = 0; i < meminfo_sz; i++) {
        if (meminfos[i]->p == p) {
            free(meminfos[i]);
            meminfos[i] = NULL;
            { // Optional: shift the remaining elements
                for (size_t j = i; j < meminfo_i - 1; j++) {
                    meminfos[j] = meminfos[j + 1];
                }
                meminfos[meminfo_i - 1] = NULL;
                meminfo_i--;
            }
            break;
        }
    }
}

void debug_mem() {
    printf("\033[33m* Currently allocated memory blocks:\033[0m\n");

    if (meminfo_i == 0) {
        printf("\033[33m* nothing!\033[0m\n");
        return;
    }

    for (size_t i = 0; i < meminfo_i; ++i) {
        if (meminfos[i] != NULL) {
            printf("\033[33m* %s:%zu: Block %zu/%zu: Size: %zu, Memory Address: %p\033[0m\n",
                   meminfos[i]->file,
                   meminfos[i]->line,
                   i,
                   meminfo_sz,
                   meminfos[i]->size,
                   meminfos[i]->p);
        }
    }
}

#endif
