#include <malloc.h>
#include "memutility.h"

void free_text(struct Text text) {
    for (int i = 0; i < text.length; i++) {
        if (!text.paragraphs[i])
            continue;
        free_paragraph(text.paragraphs[i]);
    }
    free(text.paragraphs);
}

void free_paragraph(struct Paragraph *paragraph) {
    for (int i = 0; i < paragraph->length; i++) {
        if (!paragraph->sentences[i])
            continue;
        free_sentence(paragraph->sentences[i]);
    }
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
