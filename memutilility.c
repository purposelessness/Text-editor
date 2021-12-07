#include <malloc.h>
#include "memutilility.h"

void free_text(struct Text text) {
    for (int i = 0; i < text.length; i++)
        free_paragraph(text.paragraphs[i]);
    free(text.paragraphs);
}

void free_paragraph(struct Paragraph *paragraph) {
    for (int i = 0; i < paragraph->length; i++)
        free_sentence(paragraph->sentences[i]);
    free(paragraph->sentences);
    free(paragraph);
}

void free_sentence(struct Sentence *sentence) {
    free(sentence->value);
    free(sentence);
}

void free_words(struct Words *words) {
    free(words->value[0]);
    free(words->value);
    free(words);
}
