#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct file_t {
    char* data;
    size_t size;
    bool error;
} file_t;

typedef struct result_t {
    bool error;
} result_t;

file_t read_file_content(const char* file_name);
result_t write_file_content(const char* file_path, const char* contents);

#endif
