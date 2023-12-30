#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

bool str_contains(const char contents[static 1], const char needle[static 1]) {
    return (strstr(contents, needle) != NULL);
}

char *str_content_between(char *contents, const char start[static 1], const char end[static 1]) {
    char *start_pos = strstr(contents, start);
    if (start_pos == NULL) {
        return NULL;
    }

    char *end_pos = strstr(start_pos + strlen(start), end);
    if (end_pos == NULL) {
        return NULL;
    }

    size_t length = end_pos - (start_pos + strlen(start));
    char *result = (char *)malloc(length + 1);
    strncpy(result, start_pos + strlen(start), length);
    result[length] = '\0';

    return result;
}

char *str_last(const char str[static 1], char split) {
    const char *last_occurrence = strrchr(str, split);

    if (last_occurrence == NULL) {
        // The split character isn't found in the string.
        // You can either return NULL or a copy of the original string.
        return NULL;
    }

    last_occurrence++;

    char *result = malloc(strlen(last_occurrence) + 1);
    if (result == NULL) {
        fprintf(stderr, "Failed to allocate memory in str_last().\n");
        return NULL;
    }

    strcpy(result, last_occurrence);
    return result;
}

// bool str_append(char *original[static 1], const char text_to_append[static 1]) {
//     size_t original_len = *original ? strlen(*original) : 0;
//     size_t append_len = strlen(text_to_append);

//     char *new_str = realloc(*original, original_len + (append_len * 2) + 1);
//     if (!new_str) {
//         fprintf(stderr, "Could not allocate in str_append()");
//         return false;
//     }

//     strcpy(new_str + original_len, text_to_append);
//     *original = new_str;
//     return true;
// }

char *str_concat(const char original[static 1], const char text_to_append[static 1]) {
    size_t original_len = strlen(original);
    size_t append_len = strlen(text_to_append);

    char *new_str = malloc(original_len + append_len);
    if (!new_str) {
        fprintf(stderr, "Could not allocate in str_concat()\n");
        return NULL;
    }

    strcpy(new_str, original);
    strncat(new_str, text_to_append, original_len + append_len);
    new_str[original_len + append_len] = '\0';

    return new_str;
}

char *str_trim(const char input[static 1]) {
    const char *start = input;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        // The string is all whitespace
        return strdup("");
    }

    const char *end = input + strlen(input) - 1;
    while (end > start && (isspace((unsigned char)*end) || *end == '\r' || *end == '\n')) {
        end--;
    }

    size_t length = end - start + 1;
    char *trimmed = malloc(length + 1);
    if (!trimmed) {
        fprintf(stderr, "could not allocate in str_trim()");
        return NULL;
    }

    strncpy(trimmed, start, length);
    trimmed[length] = '\0'; // Null-terminate the string

    return trimmed;
}

char **str_split(const char input[static 1], const char delimiter) {
    // Count how many elements will be extracted.
    size_t count = 0;
    const char *temp = input;
    while (*temp) {
        if (*temp == delimiter) {
            count++;
        }
        temp++;
    }

    // Allocate memory for result array of strings
    // +1 for the final NULL pointer and +1 for last string not followed by delimiter
    char **result = malloc((count + 2) * sizeof(char *));
    if (!result) {
        fprintf(stderr, "Could not allocate in str_split()");
        return NULL;
    }

    size_t index = 0;
    const char *start = input;
    for (const char *it = input;; it++) {
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
void free_str_split(char *str_array[static 1]) {
    if (str_array == NULL) {
        return;
    }
    for (char **p = str_array; *p; p++) {
        free(*p);
    }
    free(str_array);
}

char *now_date() {
    time_t current_time;
    struct tm *current_tm;
    time(&current_time);
    current_tm = localtime(&current_time);
    char *date_time = malloc(30 * sizeof(char));
    if (!date_time) {
        fprintf(stderr, "could not allocate in now_date()");
        return NULL;
    }
    strftime(date_time, 30, "%Y-%m-%d", current_tm); // %H:%M
    return date_time;
}

char *str_replace(char *src, const char *find, const char *replace) {
    // sourced from stb_dupreplace https://github.com/nothings/stb/
    // https://github.com/nothings/stb/blob/f4a71b13373436a2866c5d68f8f80ac6f0bc1ffe/tests/prerelease/stb_lib.h#L958C1-L958C59
    size_t len_find = strlen(find);
    size_t len_replace = strlen(replace);
    char *s, *p, *q;
    int count = 0;

    s = strstr(src, find);
    if (s == NULL)
        return strdup(src);
    do {
        ++count;
        s = strstr(s + len_find, find);
    } while (s != NULL);

    p = (char *)malloc(strlen(src) + count * (len_replace - len_find) + 1);
    if (p == NULL)
        return p;
    q = p;
    s = src;
    for (;;) {
        char *t = strstr(s, find);
        if (t == NULL) {
            strcpy(q, s);
            assert(strlen(p) == strlen(src) + count * (len_replace - len_find));
            return p;
        }
        memcpy(q, s, t - s);
        q += t - s;
        memcpy(q, replace, len_replace);
        q += len_replace;
        s = t + len_find;
    }
}

void str_append(str *s, const char *text_to_append, size_t size) {
    if (s->len + size > s->cap) {
        size_t new_cap = s->len + size;
        char *new_buffer = realloc(s->str, new_cap);
        if (!new_buffer) {
            fprintf(stderr, "could not increase buffer in str_append() for %s\n", text_to_append);
            exit(1);
        }
        s->str = new_buffer;
        s->cap = new_cap;
    }

    memcpy(s->str + s->len, text_to_append, size);
    s->len += size;
    s->str[s->len] = '\0';
}

void str_free(str *s) {
    free(s->str);
    s->str = NULL;
    s->len = 0;
    s->cap = 0;
}