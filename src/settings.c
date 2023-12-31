#include "settings.h"
#include "../libs/cJSON.h"
#include "files.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

void settings_parse(settings_t settings[static 1]) {
    const file_t settings_f = read_file_content("/Users/n/.config/site-gen/settings.json");

    if (settings_f.error) {
        fprintf(stderr, "could not load settings, aborting\n");
        exit(1);
    }

    cJSON* settings_j = cJSON_Parse(settings_f.data);

    if (!settings_j) {
        fprintf(stderr, "Failed to parse JSON data.\n");
        exit(1);
    }

    settings->workdir = strdup(cJSON_GetObjectItem(settings_j, "workdir")->valuestring);
    settings->webroot = strdup(cJSON_GetObjectItem(settings_j, "webroot")->valuestring);
    settings->template_article = strdup(cJSON_GetObjectItem(settings_j, "template")->valuestring);
    settings->template_index = strdup(cJSON_GetObjectItem(settings_j, "templateindex")->valuestring);
    settings->content_tag = strdup(cJSON_GetObjectItem(settings_j, "contenttag")->valuestring);
    settings->title_tag = strdup(cJSON_GetObjectItem(settings_j, "titletag")->valuestring);
    settings->description_tag = strdup(cJSON_GetObjectItem(settings_j, "descriptiontag")->valuestring);
    settings->keywords_tag = strdup(cJSON_GetObjectItem(settings_j, "keywordstag")->valuestring);

    cJSON_Delete(settings_j);
}

void settings_free(settings_t* settings) {
    free(settings->workdir);
    free(settings->webroot);
    free(settings->template_article);
    free(settings->template_index);
    free(settings->content_tag);
    free(settings->title_tag);
    free(settings->description_tag);
    free(settings->keywords_tag);
    free(settings);
}

settings_t* settings_new() {
    settings_t* settings = malloc(sizeof(settings_t));
    if (!settings) {
        fprintf(stderr, "Could not allocate memory for settings\n");
        exit(1);
    }
    *settings = (settings_t){0};
    return settings;
}