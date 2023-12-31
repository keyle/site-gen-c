#include "settings.h"
#include "md.h"
#include "files.h"
#include "article.h"
#include "sitegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#define MAX_MD 200

int main(void) {
    settings_t *settings = settings_new();
    int capacity = 200;
    int count = 0;

    if (!settings) {
        fprintf(stderr, "Could not allocate memory for settings\n");
        exit(1);
    }

    char **list = malloc(sizeof(char *) * capacity);
    if (!list) {
        fprintf(stderr, "Could not allocate memory for list\n");
        exit(1);
    }

    settings_parse(settings);
    if (settings->workdir == NULL) {
        fprintf(stderr, "Could not find workdir in settings\n");
        exit(1);
    }

    files_find_md(settings->workdir, list, &count, &capacity);

    article_t **article_list = malloc(sizeof(article_t *) * count);
    if (article_list == NULL) {
        printf("could not allocate memory for articles\n");
        exit(1);
    }

    file_t template_index = read_file_content(settings->template_index);
    if (template_index.error) {
        fprintf(stderr, "could not read template index\n");
        exit(1);
    }

    file_t template_article = read_file_content(settings->template_article);
    if (template_article.error) {
        fprintf(stderr, "could not read template article\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        const char *md_file = list[i];
        article_t *article = malloc(sizeof(article_t));
        *article = (article_t){0};
        process(settings, article, md_file, template_index.data, template_article.data);
        write_html(article);
        article_list[i] = article;
    }

    make_blog_index(settings, article_list, count);
    make_sitemap(settings, article_list, count);
    make_rss(settings, article_list, count);
    settings_free(settings);

    return 0;
}
