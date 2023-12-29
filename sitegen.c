#include "sitegen.h"
#include "utils.h"
#include "libs/md4c-html.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct str {
    size_t len;
    size_t cap;
    char* str;
} str;

void md_parse_progress(const MD_CHAR output[static 1], MD_SIZE size, void* userdata) {
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
    const char* raw_markdown = read_file_content(markdownPath);
    const char* markdown = str_trim(raw_markdown);
    article->markdown = strdup(markdown);
    article->path = strdup(markdownPath);
    article->file = str_last(markdownPath, '/');

    article->is_blog = str_contains(markdown, "<x-blog-title>");

    char* template =
        str_contains(markdown, "<x-index/>")
            ? read_file_content(settings->template_index)
            : read_file_content(settings->template_article);

    MD_SIZE size = strlen(article->markdown);
    unsigned parser_flags = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES;
    unsigned renderer_flags = 0;

    str* raw_html = &(str){0}; // used for the buffered writes by md_html in md_parse_progress
    md_html(article->markdown, size, md_parse_progress, raw_html, parser_flags, renderer_flags);

    const char* tag1 = article->is_blog ? "<x-blog-title>" : "<x-title>";
    const char* tag2 = article->is_blog ? "</x-blog-title>" : "</x-title>";

    article->title = str_content_between(article->markdown, tag1, tag2);
    article->description = str_content_between(article->markdown, "<x-desc>", "</x-desc");
    article->tags = str_content_between(article->markdown, "<x-tags>", "</x-tags");

    // vanity url = settings.webroot + (article.path - settings.workdir - article.file)
    // or in this case, we take the web root, and we append what's between the workdir and the filename,
    // that's the vanity url
    char* vanity_url = strdup(settings->webroot);
    bool res = str_append(&vanity_url, str_content_between(article->path, settings->workdir, article->file));
    if (!res) {
        fprintf(stderr, "str_append: could not write vanity url");
    }

    char* template_w_content = str_replace(template, settings->content_tag, raw_html->str);
    char* template_w_title = str_replace(template_w_content, settings->title_tag, article->title);
    char* template_w_bodyclass;

    if (article->is_blog) {
        template_w_bodyclass = str_replace(template_w_title, "<body>", "<body class='blog'>");
        article->pub_date = str_content_between(article->markdown, "<sub>", "</sub>");
    } else {
        article->pub_date = now_date();
        template_w_bodyclass = template_w_title;
    }

    char* template_w_description = str_replace(template_w_bodyclass, settings->description_tag, article->description);
    char* template_w_keywords = str_replace(template_w_description, settings->keywords_tag, article->tags);

    article->html = template_w_keywords;
}
