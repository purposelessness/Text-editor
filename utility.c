#include "utility.h"
#include <stdio.h>

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
