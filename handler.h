#ifndef COURSEWORK_HANDLER_H
#define COURSEWORK_HANDLER_H

#include "datatypes.h"

int filter_text(struct Text *text);

void print_colorized_text(struct Text text);

void print_capitalized_words(struct Text text);

void print_sorted_text(struct Text text);

void print_text_without_numbers(struct Text text);

int remove_numbers(struct Text *text);

#endif //COURSEWORK_HANDLER_H
