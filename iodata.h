#ifndef COURSEWORK_IODATA_H
#define COURSEWORK_IODATA_H

#include "datatypes.h"

struct Text txtscan();

struct Sentence *sntscan();

void txtprint(struct Text text);

void sntprint(struct Sentence *restrict sentence);

#endif //COURSEWORK_IODATA_H
