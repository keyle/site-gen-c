#include "article.h"
#include <stdlib.h>
#include <strings.h>

int compare_articles_date_descending(const void* a, const void* b) {
    Article* article1 = *(Article**)a;
    Article* article2 = *(Article**)b;
    return strcmp(article2->pub_date, article1->pub_date);
}

void sort_articles_date_descending(Article* article_list[static 1], size_t article_count) {
    qsort(article_list, article_count, sizeof(Article*), compare_articles_date_descending);
}
