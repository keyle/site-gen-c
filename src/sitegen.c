#include "sitegen.h"
#include "article.h"
#include "utils.h"
#include "files.h"
#include "../libs/md4c-html.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

void md_parse_progress(const MD_CHAR output[static 1], MD_SIZE size, void* userdata) {
    // fwrite(output, size, 1, stdout); // For example, write to stdout
    if (!userdata) {
        fprintf(stderr, "could not pass user data in process_output");
        exit(1);
    }
    str* s = userdata;
    str_append(s, output, size);
}

void process(settings_t settings[static 1], article_t article[static 1], const char markdown_path[static 1], char template_index[static 1], char template_article[static 1]) {
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
    article->url = str_concat(vanity_url, str_content_between(article->path, settings->workdir, article->file));

    char* template = str_contains(markdown, "<x-index/>") ? template_index : template_article;
    char* template_w_content = str_replace(template, settings->content_tag, str_trim(raw_html->str));
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

void write_html(article_t article[static 1]) {
    const char* html_path = str_replace(article->path, article->file, "");
    const char* target = str_concat(html_path, "index.html");
    result_t res = write_file_content(target, article->html);
    if (res.error) {
        fprintf(stderr, "Could not write HTML file, aborting.\n");
        exit(1);
    }
}

void make_blog_index(settings_t settings[static 1], article_t* article_list[static 1], size_t article_count) {
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
    str* table = &(str){0};
    str_append(table, "<table>", 7);

    for (size_t i = 0; i < article_count; i++) {
        article_t* article = article_list[i];
        if (!article->is_blog)
            continue;
        char to_add[1000];
        const char* date = article_format_date_web(article->pub_date);
        sprintf(to_add, "<tr><td>%s</td><td><a href='%s'>%s</a></td><td>&nbsp;</td>", date, article->url, article->title);
        str_append(table, to_add, strlen(to_add));
    }
    str_append(table, "</table>", 8);
    char* new_html = str_replace(template.data, "<x-blog-index/>", table->str);
    write_file_content(template_loc, new_html);
}

void make_sitemap(settings_t settings[static 1], article_t* article_list[static 1], size_t article_count) {
    const char* header =
        "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\
<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\" \
xmlns:xhtml=\"http://www.w3.org/1999/xhtml\">\n";
    str* sitemap = &(str){0};
    str_append(sitemap, header, strlen(header));

    for (size_t i = 0; i < article_count; i++) {
        article_t* article = article_list[i];
        char to_add[1000];
        sprintf(to_add, "<url><loc>%s</loc><lastmod>%s</lastmod></url>\n", article->url, article->pub_date);
        str_append(sitemap, to_add, strlen(to_add));
    }

    str_append(sitemap, "</urlset>\n", 10);
    char* sitemap_loc = str_concat(settings->workdir, "/sitemap.xml");
    write_file_content(sitemap_loc, sitemap->str);
}

void make_rss(settings_t settings[static 1], article_t* article_list[static 1], size_t article_count) {
    const char* header =
        "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n\
<rss version=\"2.0\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n\
  <channel>\n\
    <title>NobenLog</title>\n\
    <link>https://noben.org/blog/</link>\n\
    <description>Recent content on NobenLog</description>\n\
    <generator>site-gen-c -- https://github.com/keyle/site-gen-c</generator>\n\
    <language>en-us</language>";

    str* feed = &(str){0};
    str_append(feed, header, strlen(header));

    sort_articles_date_descending(article_list, article_count);

    for (size_t i = 0; i < article_count; i++) {
        article_t* article = article_list[i];
        if (!article->is_blog)
            continue;
        char to_add[3000];
        const char* pub_date = article_format_date_rss(article->pub_date);
        sprintf(to_add, "<item><title>%s</title><link>%s</link><pubDate>%s</pubDate><guid>%s</guid><description><![CDATA[ %s ]]></description></item>\n", article->title, article->url, pub_date, article->url, article->description);
        str_append(feed, to_add, strlen(to_add));
    }
    const char* footer = "</channel></rss>\n";
    str_append(feed, footer, strlen(footer));

    char* sitemap_loc = str_concat(settings->workdir, "/index.xml");
    write_file_content(sitemap_loc, feed->str);
}
