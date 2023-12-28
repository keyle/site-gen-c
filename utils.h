#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

char* read_file_content(const char* filename);
char* str_content_between(char* contents, const char* start, const char* end);
bool str_contains(const char* contents, const char* needle);
void str_replace(char* content, const char* from, const char* to);
char* now_date();

#endif
