#include <wchar.h>
#include "ioutility.h"

#define BUF_SIZE 10

wchar_t bufc[BUF_SIZE];
int bufp = 0;

wchar_t getch() {
    return bufp > 0 ? bufc[--bufp] : (wchar_t) getwchar();
}

int ungetch(wchar_t c) {
    if (bufp >= BUF_SIZE)
        return 0;
    bufc[bufp++] = (wchar_t) c;
    return 1;
}
