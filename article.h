#ifndef ARTICLE_H
#define ARTICLE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char* path;
    char* file;
    char* markdown;
    char* html;
    char* title;
    bool is_blog;
    char* pub_date;
    char* description;
    char* tags;
} Article;

void sort_articles_date_descending(Article** article_list, size_t article_count);

#endif
