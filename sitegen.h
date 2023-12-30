#ifndef SITEGEN_H
#define SITEGEN_H

#include "settings.h"
#include "article.h"

void process(Settings*, Article*, const char* markdown_path);
void write_html(Article*);
void blog_index(Settings*, Article** article_list, size_t article_count);

#endif
