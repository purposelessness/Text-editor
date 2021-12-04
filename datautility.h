#ifndef COURSEWORK_DATAUTILITY_H
#define COURSEWORK_DATAUTILITY_H

#include "datatypes.h"

int snticmp(const struct Sentence *snt1, const struct Sentence *snt2);

struct Words *sntwrds(struct Sentence sentence);

#endif //COURSEWORK_DATAUTILITY_H
