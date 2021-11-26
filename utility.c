#include "utility.h"
#include "datatypes.h"
#include <stdio.h>
#include <malloc.h>

#define BUFSIZE 10

char buf[BUFSIZE];
int bufp = 0;

int getch() {
    return bufp > 0 ? buf[--bufp] : (char) getchar();
}

int ungetch(int c) {
    if (bufp >= BUFSIZE)
        return 0;
    buf[bufp++] = (char) c;
    return 1;
}

void freeText(void *ptr) {
    struct Text *text = (struct Text *) ptr;
    for (int i = 0; i < text->length; i++)
        freeSentence(text->sentences[i]);
    free(text->sentences);
    free(text);
}

void freeSentence(void *ptr) {
    struct Sentence *sentence = (struct Sentence *) ptr;
    free(sentence->value);
    free(sentence);
}
