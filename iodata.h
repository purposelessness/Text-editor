#ifndef COURSEWORK_IODATA_H
#define COURSEWORK_IODATA_H

#include "datatypes.h"

struct Text *scanText();
struct Sentence *scanSentence();
void printText(struct Text *text);
void printSentence(struct Sentence *sentence);

#endif //COURSEWORK_IODATA_H
