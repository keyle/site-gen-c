#ifndef ARTICLE_H
#define ARTICLE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char* markdown;
    char* html;
    char* title;
    char* path;
    char* file;
    char* url;
    char* description;
    char* tags;
    char* pub_date;
    bool is_blog;
} Article;

void sort_articles_date_descending(Article** article_list, size_t article_count);
char* article_format_date_web(const char* input);
char* article_format_date_rss(const char* input);

#endif
