#ifndef COURSEWORK_DATATYPES_H
#define COURSEWORK_DATATYPES_H

#include <stddef.h>

typedef enum {
    false, true
} bool;

struct Text {
    struct Paragraph **paragraphs;
    int length;
};

struct Paragraph {
    struct Sentence **sentences;
    int length;
};

struct Sentence {
    wchar_t *value;
    int length;
};

struct Words {
    wchar_t **value;
    int length;
};

struct Word {
    wchar_t *value;
    bool flag;
    int *sentences;
    int size;
    int length;
};

#endif //COURSEWORK_DATATYPES_H
