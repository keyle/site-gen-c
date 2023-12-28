
#include "sitegen.h"
#include "utils.h"
#include "libs/md4c-html.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

void process_output(const MD_CHAR output[static 1], MD_SIZE size, void* userdata) {
    // Write or process the HTML output
    fwrite(output, size, 1, stdout); // For example, write to stdout
    if (!userdata) {
        fprintf(stderr, "could not pass user data in process_output");
        exit(1);
    }
    Article* article = userdata;
    article->html = strdup(output);
}

void process(Settings settings[static 1], Article article[static 1], const char markdownPath[static 1]) {
    const char* markdown = read_file_content(markdownPath);
    article->markdown = strdup(markdown);

    article->is_blog = str_contains(markdown, "<x-blog-title>");

    char* template =
        str_contains(markdown, "<x-index/>")
            ? read_file_content(settings->template_index)
            : read_file_content(settings->template_article);

    MD_SIZE size = strlen(article->markdown);
    unsigned parser_flags = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES;
    unsigned renderer_flags = 0; // Default renderer flags

    md_html(article->markdown, size, process_output, article, parser_flags, renderer_flags);

    str_replace(template, settings->content_tag, article->html);

    const char* tag1 = article->is_blog ? "<x-blog-title>" : "<x-title>";
    const char* tag2 = article->is_blog ? "</x-blog-title>" : "</x-title>";
    char* title = str_content_between(article->markdown, tag1, tag2);
    article->title = strdup(title);

    str_replace(template, settings->title_tag, article->title);

    if (article->is_blog) {
        str_replace(template, "<body>", "<body class='blog'>");
        article->pub_date = str_content_between(article->markdown, "<sub>", "</sub>");
    } else {
        article->pub_date = strdup(now_date());
    }

    article->html = strdup(template);
}
