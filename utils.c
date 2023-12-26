#include "utils.h"

void findMarkdownInFolders(const char *dirPath, char ***markdowns, int *i) {
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
        findMarkdownInFolders(folder, markdowns, i);
    }
    closedir(dir);
}

char *read_file_content(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 0;
    }

    // Get the file size
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);

    // Allocate memory for the file content
    char *content = (char *)malloc(file_size);
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
