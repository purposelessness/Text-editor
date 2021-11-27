#include "utility.h"
#include <malloc.h>
#include <wchar.h>

#define BUFSIZE 10

wchar_t bufc[BUFSIZE];
int bufp = 0;

wchar_t getch() {
    return bufp > 0 ? bufc[--bufp] : (wchar_t) getwchar();
}

int ungetch(wchar_t c) {
    if (bufp >= BUFSIZE)
        return 0;
    bufc[bufp++] = (wchar_t) c;
    return 1;
}

void txtfree(struct Text *restrict text) {
    for (int i = 0; i < text->length; i++)
        sntfree(text->sentences[i]);
    free(text->sentences);
}

void sntfree(struct Sentence *restrict sentence) {
    free(sentence->value);
    free(sentence);
}

int snticmp(const struct Sentence *restrict snt1, const struct Sentence *restrict snt2) {
#ifdef __linux__
    return wcsncasecmp(snt1->value, snt2->value, max((int) snt1->length, (int) snt2->length));
#elif _WIN32
    return _wcsnicmp(snt1->value, snt2->value, wcslen(snt1->value)-2);
#endif
}

int max(int a, int b) {
    return a > b ? a : b;
}
