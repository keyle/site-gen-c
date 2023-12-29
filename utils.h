#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

char* read_file_content(const char* filename);
char* str_content_between(char* contents, const char* start, const char* end);
bool str_contains(const char* contents, const char* needle);
char* str_replace(const char* contents, const char* from, const char* to);
char* str_last(const char* str, char split);
bool str_append(char** original, const char* text_to_append);
char* now_date();

#endif
