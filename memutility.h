#ifndef COURSEWORK_MEMUTILITY_H
#define COURSEWORK_MEMUTILITY_H

#include "datatypes.h"

void free_text(struct Text text);

void free_paragraph(struct Paragraph *paragraph);

void free_sentence(struct Sentence *sentence);

void free_words(struct Words *words);

#endif //COURSEWORK_MEMUTILITY_H
