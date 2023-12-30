#include "md.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void files_find_md(const char dir_path[static 1], char **list, int count[static 1], int capacity[static 1]) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_path)) == NULL) {
        printf("Could not open directory %s\n", dir_path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) {
            char *ext = strrchr(entry->d_name, '\0') - 3;
            if (!ext || ext == entry->d_name)
                continue;
            if (strcmp(ext, ".md") != 0)
                continue;
            printf("found markdown: %s, ext: %s\n", entry->d_name, ext);
            char *md = malloc(strlen(dir_path) + strlen(entry->d_name) + 3);
            strcpy(md, dir_path);
            strcat(md, "/");
            strcat(md, entry->d_name);
            if (*count == *capacity) {
                (*capacity) *= 2;
                list = realloc(list, sizeof(char *) * (*capacity));
            }
            list[(*count)++] = md;
            continue;
        }

        if (entry->d_name[0] == '.')
            continue;
        if (strcmp(entry->d_name, "..") == 0)
            continue;

        char *folder = malloc(strlen(dir_path) + strlen(entry->d_name) + 3);
        strcpy(folder, dir_path);
        strcat(folder, "/");
        strcat(folder, entry->d_name);
        files_find_md(folder, list, count, capacity);
    }

    closedir(dir);
}
