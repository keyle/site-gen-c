#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>

bool str_contains(const char contents[static 1], const char needle[static 1]) {
    return (strstr(contents, needle) != NULL);
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
        fprintf(stderr, "Failed to allocate memory in str_replace()\n");
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
        fprintf(stderr, "Could not allocate in str_append()");
        return false;
    }

    strcpy(new_str + original_len, text_to_append);
    *original = new_str;
    return true;
}

char* str_concat(const char original[static 1], const char text_to_append[static 1]) {
    size_t original_len = strlen(original);
    size_t append_len = strlen(text_to_append);

    char* new_str = malloc(original_len + append_len + 1);
    if (!new_str) {
        fprintf(stderr, "Could not allocate in str_concat()\n");
        return NULL;
    }

    strcpy(new_str, original);
    strcat(new_str, text_to_append);

    return new_str;
}

char* str_trim(const char input[static 1]) {
    const char* start = input;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        // The string is all whitespace
        return strdup("");
    }

    const char* end = input + strlen(input) - 1;
    while (end > start && (isspace((unsigned char)*end) || *end == '\r' || *end == '\n')) {
        end--;
    }

    size_t length = end - start + 1;
    char* trimmed = malloc(length + 1);
    if (!trimmed) {
        fprintf(stderr, "could not allocate in str_trim()");
        return NULL;
    }

    strncpy(trimmed, start, length);
    trimmed[length] = '\0'; // Null-terminate the string

    return trimmed;
}

char** str_split(const char input[static 1], const char delimiter) {
    // Count how many elements will be extracted.
    size_t count = 0;
    const char* temp = input;
    while (*temp) {
        if (*temp == delimiter) {
            count++;
        }
        temp++;
    }

    // Allocate memory for result array of strings
    // +1 for the final NULL pointer and +1 for last string not followed by delimiter
    char** result = malloc((count + 2) * sizeof(char*));
    if (!result) {
        fprintf(stderr, "Could not allocate in str_split()");
        return NULL;
    }

    size_t index = 0;
    const char* start = input;
    for (const char* it = input;; it++) {
        if (*it == delimiter || *it == '\0') {
            size_t length = it - start;
            result[index] = malloc(length + 1); // +1 for null terminator
            if (!result[index]) {
                // Handle allocation failure: free previously allocated memory
                while (index > 0) {
                    free(result[--index]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[index], start, length);
            result[index][length] = '\0'; // Null terminate the string
            index++;

            if (*it == '\0') {
                break;
            }
            start = it + 1;
        }
    }
    result[index] = NULL; // Mark the end of the array

    return result;
}

// Function to free the memory allocated by str_split
void free_str_split(char* str_array[static 1]) {
    if (str_array == NULL) {
        return;
    }
    for (char** p = str_array; *p; p++) {
        free(*p);
    }
    free(str_array);
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
    strftime(date_time, 30, "%Y-%m-%d", current_tm); // %H:%M
    return date_time;
}
