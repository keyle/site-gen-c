#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

char* read_file_content(const char filename[static 1]) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 0;
    }

    // Get the file size
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);

    // Allocate memory for the file content
    char* content = (char*)malloc(file_size);
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

bool str_contains(const char contents[static 1], const char needle[static 1]) {
    if (strstr(contents, needle) != NULL) {
        return true;
    }
    return false;
}

void str_replace(char content[static 1], const char from[static 1], const char to[static 1]) {
    size_t from_len = strlen(from);
    size_t to_len = strlen(to);

    char* result = malloc(strlen(content) + 1); // Dynamic allocation for the result
    char* current_pos = content;
    char* temp = result;

    while (*current_pos) {
        if (strstr(current_pos, from) == current_pos) { // Match found
            strcpy(temp, to);
            current_pos += from_len;
            temp += to_len;
        } else {
            *temp++ = *current_pos++;
        }
    }

    *temp = '\0';
    strcpy(content, result);
    free(result);
}

char* str_content_between(char* contents, const char start[static 1], const char end[static 1]) {
    char* start_pos = strstr(contents, start);
    if (start_pos == NULL) {
        return NULL;
    }

    char* end_pos = strstr(start_pos + strlen(start), end);
    if (end_pos == NULL) {
        return NULL;
    }

    size_t length = end_pos - (start_pos + strlen(start));
    char* result = (char*)malloc(length + 1);
    strncpy(result, start_pos + strlen(start), length);
    result[length] = '\0';

    return result;
}

char* now_date() {
    time_t current_time;
    struct tm* current_tm;
    time(&current_time);
    current_tm = localtime(&current_time);
    char* date_time = malloc(30 * sizeof(char));
    if (!date_time) {
        fprintf(stderr, "could not allocate in now_date()");
        return NULL;
    }
    strftime(date_time, 30, "%Y-%m-%d %H:%M", current_tm);
    return date_time;
}
