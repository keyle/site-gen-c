#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct {
    char* workdir;
    char* webroot;
    char* template_article;
    char* template_index;
    char* content_tag;
    char* title_tag;
    char* description_tag;
    char* keywords_tag;
} settings_t;

settings_t* settings_new();
void settings_parse(settings_t* settings);
void settings_free(settings_t* settings);

#endif
