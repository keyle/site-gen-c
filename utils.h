#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdlib.h>

char* str_content_between(char* contents, const char* start, const char* end);
bool str_contains(const char* contents, const char* needle);
char* str_replace(const char* contents, const char* from, const char* to);
char* str_last(const char* str, char split);

bool str_append(char** original, const char* text_to_append); // prefer using str_concat
char* str_concat(const char* original, const char* text_to_append);
char* str_trim(const char* input);

char** str_split(const char* input, const char delimiter); // use with free_str_split
void free_str_split(char** str_array);

char* now_date();

#endif
