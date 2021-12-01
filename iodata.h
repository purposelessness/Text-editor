#ifndef COURSEWORK_IODATA_H
#define COURSEWORK_IODATA_H

#include "datatypes.h"

struct Text scantxt();

struct Paragraph *scanpar();

struct Sentence *scansnt();

void printtxt(struct Text text);

void printpar(struct Paragraph *paragraph);

void printsnt(struct Sentence *sentence);

#endif //COURSEWORK_IODATA_H
