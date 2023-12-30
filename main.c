#include "settings.h"
#include "md.h"
#include "article.h"
#include "sitegen.h"
#include "augment.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#define MAX_MD 200

int main(void) {
    char **markdown_path_list = calloc(MAX_MD, sizeof(char *));
    int total_md = 0;

    if (markdown_path_list == NULL) {
        printf("could not allocate memory");
        exit(1);
    }

    Settings *settings = malloc(sizeof(Settings));
    settings_parse(settings);

    find_markdown_files(settings->workdir, &markdown_path_list, &total_md);

    for_in(char *markdown, markdown_path_list, total_md) {
        Article *article = malloc(sizeof(Article));
        *article = (Article){0};
        process(settings, article, markdown);
        write(article);
    }

    free(settings);
    free(markdown_path_list);
    return 0;
}
