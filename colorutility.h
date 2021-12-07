#ifndef COURSEWORK_COLORUTILITY_H
#define COURSEWORK_COLORUTILITY_H

#include <stddef.h>
#include "datatypes.h"

struct Text colorize_text(struct Text text);

struct Paragraph *colorize_paragraph(struct Paragraph paragraph);

struct Sentence *colorize_sentence(struct Sentence sentence);

#endif //COURSEWORK_COLORUTILITY_H
