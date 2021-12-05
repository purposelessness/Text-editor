#ifndef COURSEWORK_MEMUTILILITY_H
#define COURSEWORK_MEMUTILILITY_H

#include "datatypes.h"

void freetxt(struct Text text);

void freepar(struct Paragraph *paragraph);

void freesnt(struct Sentence *sentence);

void freewrds(struct Words *words);

void freewrd(struct Word *word);

#endif //COURSEWORK_MEMUTILILITY_H
