#ifndef COURSEWORK_HANDLER_H
#define COURSEWORK_HANDLER_H

#include "datatypes.h"

int filter_text(struct Text *text);

void print_colorized_text(const struct Text *text);

void print_capitalized_words(const struct Text *text);

void print_sorted_text(const struct Text *text);

void print_text_without_numbers(struct Text *text);

void sort_text_task(struct Text *text);

#endif //COURSEWORK_HANDLER_H
