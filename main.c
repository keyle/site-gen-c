#include "settings.h"
#include "utils.h"
#include "md.h"
#include "libs/md4c.h"
#include "libs/md4c-html.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void markdown_html(const char *);

int main() {
    // const char *dirPath = ".";
    char **markdowns = calloc(2000 * 150, sizeof(char *));
    int i = 0;
    // void *v = 0;
    // MD_PARSER *p = malloc(sizeof(MD_PARSER *));
    Settings *settings = malloc(sizeof(Settings));
    unmarshal_settings(settings);

    find_markdown_files(settings->workdir, &markdowns, &i);

    // for (int j = 0; j < i; j++) {
    //     printf("md: %s\n", markdowns[j]);
    //     char *t = "*some text*";

    //     int res = md_parse(t, strlen(t) + 1, p, v);
    //     printf("%i\n", res);
    // }

    // TODO @next check the process in ocaml and rewrite it in C

    markdown_html("# Hello, Newb!\n\nThis is a simple test. <x-title>this was already a title</x-title>");

    return 0;
}

void process_output(const MD_CHAR *output, MD_SIZE size, void *userdata) {
    // Write or process the HTML output
    fwrite(output, size, 1, stdout); // For example, write to stdout
}

void markdown_html(const char *markdown_text) {
    // const char *markdownText = "# Hello, Markdown!\nThis is a simple test. <x-title>this was already a title</x-title>";
    MD_SIZE textSize = strlen(markdown_text);

    unsigned parser_flags = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES;
    unsigned renderer_flags = 0; // Default renderer flags

    // Call the md_html function
    md_html(markdown_text, textSize, process_output, NULL, parser_flags, renderer_flags);
}
