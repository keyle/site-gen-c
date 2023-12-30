#include "sitegen.h"
#include "article.h"
#include "utils.h"
#include "files.h"
#include "libs/md4c-html.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

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

void process(Settings settings[static 1], Article article[static 1], const char markdown_path[static 1]) {
    const file_t loaded = read_file_content(markdown_path);
    if (loaded.error) {
        fprintf(stderr, "Could not read file %s, aborting.\n", markdown_path);
        exit(1);
    }
    const char* raw_markdown = loaded.data;
    const char* markdown = str_trim(raw_markdown);
    article->markdown = strdup(markdown);
    article->path = strdup(markdown_path);
    article->file = str_last(markdown_path, '/');

    article->is_blog = str_contains(markdown, "<x-blog-title>");

    MD_SIZE size = strlen(article->markdown);
    unsigned parser_flags = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES;
    unsigned renderer_flags = 0;

    str* raw_html = &(str){0}; // used for the buffered writes by md_html in md_parse_progress
    md_html(article->markdown, size, md_parse_progress, raw_html, parser_flags, renderer_flags);

    const char* tag1 = article->is_blog ? "<x-blog-title>" : "<x-title>";
    const char* tag2 = article->is_blog ? "</x-blog-title>" : "</x-title>";
    if (!tag1 || !tag2) {
        fprintf(stderr, "could not find any title tags for %s\n", article->path);
        exit(1);
    }

    article->title = str_content_between(article->markdown, tag1, tag2);
    if (!article->title) {
        fprintf(stderr, "could not find <x-title> for %s\n", article->path);
        exit(1);
    }
    article->description = str_content_between(article->markdown, "<x-desc>", "</x-desc");
    if (!article->description) {
        fprintf(stderr, "could not find <x-desc> for %s\n", article->path);
        exit(1);
    }
    article->tags = str_content_between(article->markdown, "<x-tags>", "</x-tags");
    if (!article->tags) {
        fprintf(stderr, "could not find <x-tags> for %s\n", article->path);
        exit(1);
    }

    // vanity url = settings.webroot + (article.path - settings.workdir - article.file)
    // or in this case, we take the web root, and we append what's between the workdir and the filename,
    // that's the vanity url
    char* vanity_url = strdup(settings->webroot);
    bool res = str_append(&vanity_url, str_content_between(article->path, settings->workdir, article->file));
    if (!res) {
        fprintf(stderr, "str_append: could not write vanity url");
    }

    const file_t template =
        str_contains(markdown, "<x-index/>")
            ? read_file_content(settings->template_index)
            : read_file_content(settings->template_article);

    if (template.error) {
        fprintf(stderr, "could not load template, aborting");
        exit(1);
    }

    char* template_w_content = str_replace(template.data, settings->content_tag, raw_html->str);
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

void write_html(Article article[static 1]) {
    const char* html_path = str_replace(article->path, article->file, "");
    const char* target = str_concat(html_path, "index.html");
    result_t res = write_file_content(target, article->html);
    if (res.error) {
        fprintf(stderr, "Could not write HTML file, aborting.\n");
        exit(1);
    } else {
        // TODO one of the files had garbage at the bottom, might be a problem there
        //      potentially related to UTF-8 in the string?
        printf("%s: wrote %zu bytes\n", target, strlen(article->html));
    }
}

void blog_index(Settings settings[static 1], Article* article_list[static 1], size_t article_count) {
    char* template_loc = str_concat(settings->workdir, "/index.html");
    if (!template_loc) {
        fprintf(stderr, "could not allocate memory for template location in blog_index\n");
        exit(1);
    }
    const file_t template = read_file_content(template_loc);
    if (template.error) {
        fprintf(stderr, "could not load template, aborting\n");
        exit(1);
    }

    sort_articles_date_descending(article_list, article_count);
}
