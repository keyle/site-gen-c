#include "settings.h"
#include "md.h"
#include "article.h"
#include "sitegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#define MAX_MD 200

int main(void) {
    Settings *settings = malloc(sizeof(Settings));
    settings = &(Settings){0};
    int capacity = 200;
    int count = 0;

    if (!settings) {
        fprintf(stderr, "could not allocate memory for settings");
        exit(1);
    }

    char **list = malloc(sizeof(char *) * capacity);
    if (!list) {
        fprintf(stderr, "could not allocate memory for list");
        exit(1);
    }

    settings_parse(settings);
    if (settings->workdir == NULL) {
        fprintf(stderr, "could not find workdir");
        exit(1);
    }

    files_find_md(settings->workdir, list, &count, &capacity);

    printf("processed %d files\n", count);

    Article **article_list = malloc(sizeof(Article *) * count);
    if (article_list == NULL) {
        printf("could not allocate memory for articles");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        char *md_file = list[i];
        Article *article = malloc(sizeof(Article));
        process(settings, article, md_file);
        write_html(article);
        article_list[i] = article;
    }

    blog_index(settings, article_list, count);

    return 0;
}
