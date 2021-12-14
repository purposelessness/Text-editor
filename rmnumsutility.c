#include "rmnumsutility.h"
#include <wchar.h>
#include <wctype.h>
#include <malloc.h>
#include "datautility.h"
#include "memutility.h"
#include "utility.h"

struct Text txtrmnums(const struct Text *text) {
    struct Text txt;
    struct Paragraph **srcpars = text->paragraphs, **pars;
    int len = text->length;

    pars = malloc(len * sizeof(struct Paragraph *));
    if (!pars) {
        wprintf(L"Memory allocation error");
        txt.length = 0;
        return txt;
    }

    for (int i = 0; i < len; i++)
        pars[i] = parrmnums(srcpars[i]);

    txt.paragraphs = pars;
    txt.length = len;
    return txt;
}

struct Paragraph *parrmnums(const struct Paragraph *paragraph) {
    struct Paragraph *par;
    struct Sentence **snts, **srcsnts = paragraph->sentences;
    int len = paragraph->length;

    par = malloc(sizeof(struct Paragraph));
    if (!par) {
        wprintf(L"Memory allocation error");
        return NULL;
    }

    snts = malloc(len * sizeof(struct Sentence *));
    if (!snts) {
        wprintf(L"Memory allocation error");
        free(par);
        return NULL;
    }

    for (int i = 0; i < len; i++)
        snts[i] = sntrmnums(srcsnts[i]);

    par->sentences = snts;
    par->length = len;
    return par;
}

struct Sentence *sntrmnums(const struct Sentence *sentence) {
    struct Words *wrds = sntwrds(sentence);
    if (!wrds)
        return NULL;
    int strlen = sentence->length;

    struct Sentence *snt = malloc(sizeof(struct Sentence));
    if (!snt) {
        wprintf(L"Memory allocation error");
        goto err_free_wrds;
    }

    wchar_t *str = malloc(strlen * sizeof(wchar_t));
    if (!str) {
        wprintf(L"Memory allocation error");
        goto err_free_snt;
    }
    wcsncpy(str, sentence->value, strlen);

    wchar_t *ptr = str;
    bool start = true;
    for (int k = 0; k < wrds->length; k++) {
        wchar_t *wrd = wrds->value[k];
        int wrdlen = (int) wcslen(wrd);
        if (isnumber(wrd) == false) {
            ptr = str + wrdlen;
            start = false;
            continue;
        }
        wchar_t *pnum = wcsstr(ptr, wrd);
        int shift = iswpunct(*(pnum + wrdlen)) ? 0 : 1;
        if (start == true) {
            if (k + 1 == wrds->length)
                goto err_free_str;

            ptr = wmemmove(pnum, pnum + wrdlen - shift + 2, strlen - wrdlen - 2 + shift);
            strlen -= wrdlen + 2 - shift;
            continue;
        }
        ptr = wmemmove(pnum - 1 + shift, pnum + wrdlen + shift, wcslen(pnum) + 1 - wrdlen - shift);
        strlen -= wrdlen + 1;
    }

    wchar_t *buf;
    if (!(buf = realloc(str, strlen * sizeof(wchar_t)))) {
        wprintf(L"Memory reallocation error");
        goto err_free_str;
    }
    str = buf;
    free_words(wrds);

    snt->value = str;
    snt->length = strlen;

    return snt;

    err_free_str:
    free(str);
    err_free_snt:
    free(snt);
    err_free_wrds:
    free_words(wrds);
    return NULL;
}
