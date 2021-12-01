#ifndef COURSEWORK_UTILITY_H
#define COURSEWORK_UTILITY_H

#include <stddef.h>
#include "datatypes.h"

wchar_t getch();

int ungetch(wchar_t c);

void freetxt(struct Text text);

void freepar(struct Paragraph *paragraph);

void freesnt(struct Sentence *sentence);

int snticmp(const struct Sentence *snt1, const struct Sentence *snt2);

int max(int a, int b);

struct Words sntwrds(struct Sentence sentence);

#endif //COURSEWORK_UTILITY_H
