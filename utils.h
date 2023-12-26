#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void findMarkdownInFolders(const char *dirPath, char ***markdowns, int *i);
char *read_file_content(const char *filename);

#endif
