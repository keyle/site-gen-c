#include "sitegen.h"
#include "utils.h"
#include "libs/md4c-html.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct str {
    size_t len;
    size_t cap;
    char* str;
} str;

void process_output(const MD_CHAR output[static 1], MD_SIZE size, void* userdata) {
    // fwrite(output, size, 1, stdout); // For example, write to stdout

    if (!userdata) {
        fprintf(stderr, "could not pass user data in process_output");
        exit(1);
    }

    str* s = userdata;

    if (s->len + size > s->cap) {
        size_t new_cap = s->len + size; // +1
        char* new_buffer = realloc(s->str, new_cap);
        if (!new_buffer) {
            fprintf(stderr, "could not increase html buffer");
            exit(1);
        }
        s->str = new_buffer;
        s->cap = new_cap;
    }

    memcpy(s->str + s->len, output, size);
    s->len += size;
    s->str[s->len] = '\0';
}

void process(Settings settings[static 1], Article article[static 1], const char markdownPath[static 1]) {
    const char* markdown = read_file_content(markdownPath);
    article->markdown = strdup(markdown);
    article->path = strdup(markdownPath);
    article->file = str_last(markdownPath, '/');

    printf("copying raw markdown\n");
    article->is_blog = str_contains(markdown, "<x-blog-title>");

    printf("checked it contains x-blog-title\n");
    char* template =
        str_contains(markdown, "<x-index/>")
            ? read_file_content(settings->template_index)
            : read_file_content(settings->template_article);
    printf("checked it contains x-index\n");

    MD_SIZE size = strlen(article->markdown);
    unsigned parser_flags = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES;
    unsigned renderer_flags = 0; // Default renderer flags

    str* raw_html = &(str){0};

    md_html(article->markdown, size, process_output, raw_html, parser_flags, renderer_flags);
    printf("parsed the markdown to process\n");

    char* template_w_content = str_replace(template, settings->content_tag, raw_html->str);
    printf("replace content_tag in html\n");

    const char* tag1 = article->is_blog ? "<x-blog-title>" : "<x-title>";
    const char* tag2 = article->is_blog ? "</x-blog-title>" : "</x-title>";
    char* title = str_content_between(article->markdown, tag1, tag2);
    article->title = strdup(title);
    printf("set title from markdown in struct\n");

    char* template_w_title = str_replace(template_w_content, settings->title_tag, article->title);
    printf("replaced title in html\n");

    char* template_w_bodyclass;
    if (article->is_blog) {
        template_w_bodyclass = str_replace(template_w_title, "<body>", "<body class='blog'>");
        article->pub_date = str_content_between(article->markdown, "<sub>", "</sub>");
        printf("set pudate from sub\n");
    } else {
        article->pub_date = strdup(now_date());
        template_w_bodyclass = template_w_title;
        printf("set pudate from now\n");
    }

    article->html = strdup(template_w_bodyclass);
    printf("copied html over\n");
}
