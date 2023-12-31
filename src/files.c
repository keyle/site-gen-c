#include "files.h"
#include <stdio.h>
#include <strings.h>

file_t read_file_content(const char file_name[static 1]) {
    FILE* fp = fopen(file_name, "r");

    if (!fp) {
        fprintf(stderr, "Failed to open file: %s in read_file_content()\n", file_name);
        return (file_t){.error = true};
    }

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);

    char* content = (char*)malloc(file_size + 1);
    if (!content) {
        fprintf(stderr, "Failed to allocate memory in read_file_content()\n");
        fclose(fp);
        return (file_t){.error = true};
    }

    size_t bytes_read = fread(content, sizeof(char), file_size, fp);
    if (bytes_read != file_size) {
        fprintf(stderr, "Failed to read the entire file in read_file_content()\n");
        free(content);
        fclose(fp);
        return (file_t){.error = true};
    }

    content[file_size] = '\0'; // bug fix, sometimes we would keep going!

    fclose(fp);
    return (file_t){.data = content, .size = file_size, .error = false};
}

result_t write_file_content(const char file_path[static 1], const char contents[static 1]) {
    FILE* file = fopen(file_path, "w");

    if (file == NULL) {
        fprintf(stderr, "Failed to open file in write_file_content()\n");
        return (result_t){.error = true};
    }

    if (fputs(contents, file) == EOF) {
        fclose(file);
        fprintf(stderr, "Failed to write to file_path in write_file_content()\n");
        return (result_t){.error = true};
    }

    fclose(file);
    return (result_t){.error = false};
}
