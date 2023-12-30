#ifndef MD_H
#define MD_H

void find_markdown_files(const char *dirPath, char ***markdowns, int *i);
void files_find_md(const char dir_path[static 1], char *list[static 1], int count[static 1], int capacity[static 1]);

#endif
