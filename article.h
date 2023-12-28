#ifndef ARTICLE_H
#define ARTICLE_H

#include <stdbool.h>

typedef struct {
    char* path;
    char* file;
    char* markdown;
    char* html;
    char* title;
    bool is_blog;
    char* pub_date;
    char* description;
    char** tags;
} Article;

#endif
