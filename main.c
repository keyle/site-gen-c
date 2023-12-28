#include "mem.h"
#include "settings.h"
#include "md.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#define MAX_MD 200

int main(void) {
    // const char *dirPath = ".";
    char **markdowns = calloc(MAX_MD, sizeof(char *));
    if (markdowns == NULL) {
        printf("could not allocate memory");
        exit(1);
    }

    int i = 0;
    // MD_PARSER *p = malloc(sizeof(MD_PARSER *));
    Settings *settings = malloc(sizeof(Settings));
    unmarshal_settings(settings);
    //
    find_markdown_files(settings->workdir, &markdowns, &i);
    // find_markdown_files(".", &markdowns, &i);

    for (int j = 0; j < i; j++) {
        printf("md: %s\n", markdowns[j]);
        // char *t = "*some text*";
        // int res = md_parse(t, strlen(t) + 1, p, v);
        // printf("%i\n", res);
    }

    // TODO @next check the process in ocaml and rewrite it in C

    // markdown_html("# Hello, Newb!\n\nThis is a simple test. <x-title>this was already a title</x-title>");
    free(settings);
    free(markdowns);
    debug_mem();
    return 0;
}

// void process_output(const MD_CHAR *output, MD_SIZE size, void *userdata) {
//     // Write or process the HTML output
//     fwrite(output, size, 1, stdout); // For example, write to stdout
// }
//
// void markdown_html(const char *markdown_text) {
//     // const char *markdownText = "# Hello, Markdown!\nThis is a simple test. <x-title>this was already a title</x-title>";
//     MD_SIZE textSize = strlen(markdown_text);
//
//     unsigned parser_flags = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES;
//     unsigned renderer_flags = 0; // Default renderer flags
//
//     // Call the md_html function
//     md_html(markdown_text, textSize, process_output, NULL, parser_flags, renderer_flags);
// }
