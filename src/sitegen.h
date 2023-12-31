#ifndef SITEGEN_H
#define SITEGEN_H

#include "settings.h"
#include "article.h"

void process(settings_t*, article_t*, const char* markdown_path, char* template_index, char* template_article);
void write_html(article_t*);
void make_blog_index(settings_t*, article_t** article_list, size_t article_count);
void make_sitemap(settings_t*, article_t** article_list, size_t article_count);
void make_rss(settings_t*, article_t** article_list, size_t article_count);

#endif
