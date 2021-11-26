#include <malloc.h>
#include "datatypes.h"
#include "iodata.h"
#include <stdio.h>

int main() {
//    struct Text *text = scanText();
//    for (size_t i = 0; i < text->length; i++){
//        puts(text->sentences[i].value);
//        printf("input %d %d;\n", (int) i, (int) text->length);
//    }
//    for (size_t i = 0; i < text->length; i++) {
//        free(text->sentences[i].value);
//    }
//    free(text->sentences);
//    free(text);
//    struct Sentence *sentence;
//    while ((sentence = scanSentence())) {
//        free(sentence->value);
//    }
//    free(sentence);
    struct Sentence *sentence;
    while ((sentence = scanSentence())) {
        puts(sentence->value);
        free(sentence->value);
        free(sentence);
    }

    return 0;
}
