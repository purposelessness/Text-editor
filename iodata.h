#ifndef COURSEWORK_IODATA_H
#define COURSEWORK_IODATA_H

#include "datatypes.h"

struct Text scan_text();

struct Paragraph *scan_paragraph();

struct Sentence *scan_sentence();

void print_text(struct Text text);

void print_paragraph(struct Paragraph *paragraph);

void print_sentence(struct Sentence *sentence);

#endif //COURSEWORK_IODATA_H
