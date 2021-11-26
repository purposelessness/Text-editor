#ifndef COURSEWORK_UTILITY_H
#define COURSEWORK_UTILITY_H

#include <stddef.h>
#include "datatypes.h"

wchar_t getch();
int ungetch(wchar_t c);
void freeText(struct Text *text);
void freeSentence(struct Sentence *sentence);

#endif //COURSEWORK_UTILITY_H
