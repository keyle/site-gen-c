#ifndef SITEGEN_H
#define SITEGEN_H

#include "settings.h"
#include "article.h"

void process(Settings*, Article*, const char* markdown_path, char* template_index, char* template_article);
void write_html(Article*);
void make_blog_index(Settings*, Article** article_list, size_t article_count);
void make_sitemap(Settings*, Article** article_list, size_t article_count);
void make_rss(Settings*, Article** article_list, size_t article_count);
void clean_up(Settings*);

#endif
