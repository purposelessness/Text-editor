#ifndef COURSEWORK_UTILITY_H
#define COURSEWORK_UTILITY_H

#include <stddef.h>

wchar_t getch();
int ungetch(wchar_t c);
void freeText(void *ptr);
void freeSentence(void *ptr);

#endif //COURSEWORK_UTILITY_H
