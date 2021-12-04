#include <wchar.h>
#include <malloc.h>
#include "datautility.h"
#include "colorutility.h"
#include "memutilility.h"

#define DEFAULT L"\033[0m"
#define RED L"\033[31m"
#define BLUE L"\033[34m"
#define GREEN L"\033[32m"
#define YELLOW L"\033[33m"

wchar_t *wrdclr(int wrdlen) {
    switch (wrdlen % 4) {
        case 0:
            return RED;
        case 1:
            return BLUE;
        case 2:
            return GREEN;
        case 3:
            return YELLOW;
        default:
            return DEFAULT;
    }
}

wchar_t *strcolor(const struct Sentence *snt) {
    struct Words *wrds = sntwrds(*snt);
    int strlen = snt->length, wrdlen;
    wchar_t *pwrd = NULL, *wrd, *color, *str;

    str = malloc((strlen + 9 * wrds->length) * sizeof(wchar_t));
    if (str == NULL) {
        wprintf(L"Memory allocation error");

        return NULL;
    }
    wcsncpy(str, snt->value, strlen);

    for (int i = 0; i < wrds->length; i++) {
        wrd = wrds->value[i];
        wrdlen = (int) wcslen(wrd);
        color = wrdclr(wrdlen);
        pwrd = !pwrd ? wcsstr(str, wrd) : wcsstr(pwrd, wrd);

        pwrd = wmemmove(pwrd + 5, pwrd, strlen);
        wcsncpy(pwrd - 5, color, 5);
        pwrd = wmemmove(pwrd + wrdlen + 4, pwrd + wrdlen, strlen);
        wcsncpy(pwrd - 4, DEFAULT, 4);
        strlen -= wrdlen;
    }
    freewrds(wrds);

    return str;
}
