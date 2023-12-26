#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct {
    char* workdir;
    char* webroot;
    char* template_post;
    char* template_index;
    char* content_tag;
    char* title_tag;
    char* description_tag;
    char* keywords_tag;
} Settings;

void unmarshal_settings(Settings* settings);

#endif
