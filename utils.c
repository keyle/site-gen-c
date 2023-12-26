#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file_content(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 0;
    }

    // Get the file size
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);

    // Allocate memory for the file content
    char *content = (char *)malloc(file_size);
    if (!content) {
        fprintf(stderr, "Failed to allocate memory.\n");
        fclose(fp);
        return 0;
    }

    // Read the file content
    size_t bytes_read = fread(content, 1, file_size, fp);
    if (bytes_read != file_size) {
        fprintf(stderr, "Failed to read the entire file.\n");
        free(content);
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return content;
}
