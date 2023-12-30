#include "article.h"
#include <stdlib.h>
#include <strings.h>
#include <time.h>

int compare_articles_date_descending(const void* a, const void* b) {
    Article* article1 = *(Article**)a;
    Article* article2 = *(Article**)b;
    return strcmp(article2->pub_date, article1->pub_date);
}

void sort_articles_date_descending(Article* article_list[static 1], size_t article_count) {
    qsort(article_list, article_count, sizeof(Article*), compare_articles_date_descending);
}

char* article_format_date(const char* input) {
    struct tm tm;
    char* output = malloc(50); // Allocate memory for the output string
    if (!output)
        return NULL; // Check for allocation failure

    if (strptime(input, "%Y-%m-%d", &tm)) {
        strftime(output, 50, "%b %d, %Y", &tm);
    } else {
        snprintf(output, 50, "Invalid date");
    }

    return output;
}