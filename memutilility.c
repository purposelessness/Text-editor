#include <malloc.h>
#include "memutilility.h"

void freetxt(struct Text text) {
    for (int i = 0; i < text.length; i++)
        freepar(text.paragraphs[i]);
    free(text.paragraphs);
}

void freepar(struct Paragraph *paragraph) {
    for (int i = 0; i < paragraph->length; i++)
        freesnt(paragraph->sentences[i]);
    free(paragraph->sentences);
    free(paragraph);
}

void freesnt(struct Sentence *sentence) {
    free(sentence->value);
    free(sentence);
}

void freewrds(struct Words *words) {
    free(words->value[0]);
    free(words->value);
    free(words);
}
