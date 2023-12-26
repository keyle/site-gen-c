#include "md.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void find_markdown_files(const char *dirPath, char ***markdowns, int *i) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dirPath)) == NULL) {
        printf("Could not open directory %s\n", dirPath);
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
            char *md = malloc(strlen(dirPath) + strlen(entry->d_name) + 3);
            strcpy(md, dirPath);
            strcat(md, "/");
            strcat(md, entry->d_name);
            (*markdowns)[(*i)++] = md;
            continue;
        }
        if (strcmp(entry->d_name, ".git") == 0)
            continue;
        if (strcmp(entry->d_name, ".") == 0)
            continue;
        if (strcmp(entry->d_name, "..") == 0)
            continue;

        // printf("Found folder: %s\n", entry->d_name);

        char *folder = malloc(strlen(dirPath) + strlen(entry->d_name) + 3);
        strcpy(folder, dirPath);
        strcat(folder, "/");
        strcat(folder, entry->d_name);
        find_markdown_files(folder, markdowns, i);
    }
    closedir(dir);
}
