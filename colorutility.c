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

wchar_t *wrdclr(int wrdlen);

struct Text txtcolor(struct Text text) {
    struct Text txt;
    struct Paragraph **srcpars = text.paragraphs, **pars;
    int len = text.length;

    pars = malloc(len * sizeof(struct Paragraph *));
    if (pars == NULL) {
        wprintf(L"Memory allocation error");
        txt.length = 0;
        return txt;
    }

    for (int i = 0; i < len; i++) {
        pars[i] = parcolor(*srcpars[i]);

        if (pars[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(pars[j]);
            free(pars);
            txt.length = 0;
            return txt;
        }
    }

    txt.paragraphs = pars;
    txt.length = len;
    return txt;
}

struct Paragraph *parcolor(const struct Paragraph paragraph) {
    struct Paragraph *par;
    struct Sentence **snts, **srcsnts = paragraph.sentences;
    int len = paragraph.length;

    par = malloc(sizeof(struct Paragraph));
    if (par == NULL) {
        wprintf(L"Memory allocation error");
        return NULL;
    }

    snts = malloc(len * sizeof(struct Sentence *));
    if (snts == NULL) {
        wprintf(L"Memory allocation error");
        free(par);
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        snts[i] = sntcolor(*srcsnts[i]);

        if (snts[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(snts[j]);
            free(snts);
            return NULL;
        }
    }

    par->sentences = snts;
    par->length = len;
    return par;
}

struct Sentence *sntcolor(const struct Sentence sentence) {
    struct Sentence *snt;
    struct Words *wrds = sntwrds(sentence);
    int strlen = sentence.length, wrdlen;
    wchar_t *pwrd = NULL, *wrd, *color, *str;

    snt = malloc(sizeof(struct Sentence));
    if (snt == NULL) {
        wprintf(L"Memory allocation error");
        return NULL;
    }

    str = malloc((strlen + 9 * wrds->length) * sizeof(wchar_t));
    if (str == NULL) {
        wprintf(L"Memory allocation error");
        free(snt);
        return NULL;
    }
    wcsncpy(str, sentence.value, strlen);

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

    snt->value = str;
    snt->length = (int) wcslen(str);

    return snt;
}

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
