#include <stdio.h>
#include <malloc.h>
#include "datatypes.h"
#include "utility.h"
#include "iodata.h"

int main() {
    struct Text *text = scanText();
    if (text == NULL)
        return 0;
    for (int i = 0; i < text->length; i++)
        printf("%s", text->sentences[i]->value);
    freeText(text);
//    struct Sentence *sentence;
//    while ((sentence = scanSentence())) {
//        puts(sentence->value);
//        free(sentence->value);
//        free(sentence);
//    }

    return 0;
}
