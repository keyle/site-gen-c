#include "article.h"
#include <stdlib.h>
#include <strings.h>
#include <time.h>

int compare_articles_date_descending(const void* a, const void* b) {
    const article_t* article1 = *(article_t**)a;
    const article_t* article2 = *(article_t**)b;
    return strcmp(article2->pub_date, article1->pub_date);
}

void sort_articles_date_descending(article_t* article_list[static 1], size_t article_count) {
    qsort(article_list, article_count, sizeof(article_t*), compare_articles_date_descending);
}

// Format blog dates for the index.html
char* article_format_date_web(const char input[static 1]) {
    struct tm tm;
    char* output = malloc(50); // Allocate memory for the output string
    if (!output) {
        fprintf(stderr, "Could not allocate memory for date string in article_format_date_web()\n");
        exit(1);
    }

    if (strptime(input, "%Y-%m-%d", &tm)) {
        strftime(output, 50, "%b %d, %Y", &tm);
    } else {
        snprintf(output, 50, "Invalid date");
    }

    return output;
}

// Format blog posts dates using RFC 822 format
char* article_format_date_rss(const char input[static 1]) {
    struct tm tm = {0};
    char* output = malloc(100);
    if (!output) {
        fprintf(stderr, "Could not allocate memory for date string in article_format_date_rss()\n");
        exit(1);
    }

    if (strptime(input, "%Y-%m-%d", &tm)) {
        // Set default time values as strptime won't set these
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        tm.tm_isdst = -1; // Not set by strptime

        strftime(output, 100, "%a, %d %b %Y %H:%M:%S GMT", &tm);
    } else {
        snprintf(output, 100, "Invalid date");
    }

    return output;
}