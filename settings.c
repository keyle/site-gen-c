#include "settings.h"
#include "libs/cJSON.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

void unmarshal_settings(Settings *settings) {
    char *settings_json = read_file_content("/Users/n/.config/site-gen/settings.json");

    cJSON *root = cJSON_Parse(settings_json);

    if (!root) {
        fprintf(stderr, "Failed to parse JSON data.\n");
        exit(1);
    }

    settings->workdir = strdup(cJSON_GetObjectItem(root, "workdir")->valuestring);
    settings->webroot = strdup(cJSON_GetObjectItem(root, "webroot")->valuestring);
    settings->template_article = strdup(cJSON_GetObjectItem(root, "template")->valuestring);
    settings->template_index = strdup(cJSON_GetObjectItem(root, "templateindex")->valuestring);
    settings->content_tag = strdup(cJSON_GetObjectItem(root, "contenttag")->valuestring);
    settings->title_tag = strdup(cJSON_GetObjectItem(root, "titletag")->valuestring);
    settings->description_tag = strdup(cJSON_GetObjectItem(root, "descriptiontag")->valuestring);
    settings->keywords_tag = strdup(cJSON_GetObjectItem(root, "keywordstag")->valuestring);

    free(settings_json);
    // Free the memory
    cJSON_Delete(root);
}
