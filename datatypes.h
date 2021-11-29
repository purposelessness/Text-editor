#ifndef COURSEWORK_DATATYPES_H
#define COURSEWORK_DATATYPES_H

#include <stddef.h>

struct Text {
    struct Sentence **sentences;
    size_t length;
};

struct Sentence {
    wchar_t *value;
    size_t length;
};

struct Words {
    wchar_t **value;
    size_t length;
};

#endif //COURSEWORK_DATATYPES_H
