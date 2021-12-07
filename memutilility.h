#ifndef COURSEWORK_MEMUTILILITY_H
#define COURSEWORK_MEMUTILILITY_H

#include "datatypes.h"

void free_text(struct Text text);

void free_paragraph(struct Paragraph *paragraph);

void free_sentence(struct Sentence *sentence);

void free_words(struct Words *words);

#endif //COURSEWORK_MEMUTILILITY_H
