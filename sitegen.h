#ifndef SITEGEN_H
#define SITEGEN_H

#include "settings.h"
#include "article.h"

void process(Settings*, Article*, const char* markdown_path);
void write(Article*);

#endif
