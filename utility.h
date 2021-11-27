#ifndef COURSEWORK_UTILITY_H
#define COURSEWORK_UTILITY_H

#include <stddef.h>
#include "datatypes.h"

wchar_t getch();

int ungetch(wchar_t c);

void txtfree(struct Text text);

void sntfree(struct Sentence *restrict sentence);

int snticmp(const struct Sentence *restrict snt1, const struct Sentence *restrict snt2);

int max(int a, int b);

#endif //COURSEWORK_UTILITY_H
