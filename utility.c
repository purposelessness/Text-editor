#include "utility.h"
#include <malloc.h>
#include <wchar.h>

#define BUFSIZE 10

wchar_t buf[BUFSIZE];
int bufp = 0;

wchar_t getch() {
    return bufp > 0 ? buf[--bufp] : (wchar_t) getwchar();
}

int ungetch(wchar_t c) {
    if (bufp >= BUFSIZE)
        return 0;
    buf[bufp++] = (wchar_t) c;
    return 1;
}

void freeText(struct Text *text) {
    for (int i = 0; i < text->length; i++)
        freeSentence(text->sentences[i]);
    free(text->sentences);
    free(text);
}

void freeSentence(struct Sentence *sentence) {
    free(sentence->value);
    free(sentence);
}
