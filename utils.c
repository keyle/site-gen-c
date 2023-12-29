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

char* str_replace(const char content[static 1], const char from[static 1], const char to[static 1]) {
    size_t content_len = strlen(content);
    size_t from_len = strlen(from);
    size_t to_len = strlen(to);

    // Calculate the length of the result string
    size_t result_len = content_len;
    for (const char* p = content; (p = strstr(p, from)) != NULL; p += from_len) {
        result_len += (to_len - from_len);
    }

    // Allocate memory for the result
    char* result = malloc(result_len + 1);
    if (!result) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    char* temp = result;
    while (*content) {
        if (strstr(content, from) == content) {
            memcpy(temp, to, to_len);
            content += from_len;
            temp += to_len;
        } else {
            *temp++ = *content++;
        }
    }

    *temp = '\0';
    return result;
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

char* str_last(const char str[static 1], char split) {
    const char* last_occurrence = strrchr(str, split);

    if (last_occurrence == NULL) {
        // The split character isn't found in the string.
        // You can either return NULL or a copy of the original string.
        return NULL;
    }

    last_occurrence++;

    char* result = malloc(strlen(last_occurrence) + 1);
    if (result == NULL) {
        fprintf(stderr, "Failed to allocate memory in str_last().\n");
        return NULL;
    }

    strcpy(result, last_occurrence);
    return result;
}

bool str_append(char* original[static 1], const char text_to_append[static 1]) {
    size_t original_len = *original ? strlen(*original) : 0;
    size_t append_len = strlen(text_to_append);

    char* new_str = realloc(*original, original_len + (append_len * 2) + 1);
    if (!new_str) {
        fprintf(stderr, "Could not allocate in append()");
        return false;
    }

    strcpy(new_str + original_len, text_to_append);
    *original = new_str;
    return true;
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
