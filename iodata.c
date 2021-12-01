#include "iodata.h"
#include "utility.h"
#include <malloc.h>
#include <ctype.h>
#include <wchar.h>

struct Text scantxt() {
    struct Text text;
    int len = 0, buf_size = 2, size_step = 4;
    struct Paragraph **pars, **buf, *par;

    pars = malloc(buf_size * sizeof(struct Paragraph *));
    if (pars == NULL) {
        wprintf(L"Memory allocation error");
        goto err;
    }

    while ((par = scanpar())) {
        if (len == buf_size) {
            if (!(buf = realloc(pars, (buf_size += size_step) * sizeof(struct Paragraph *)))) {
                wprintf(L"Memory reallocation error\n");
                goto err_free_pars;
            }
            pars = buf;
        }
        pars[len++] = par;
    }
    if (len == 0)
        goto err_free_pars;

    if (len != buf_size) {
        if (!(buf = realloc(pars, len * sizeof(struct Paragraph *)))) {
            wprintf(L"Memory reallocation error\n");
            goto err_free_pars;
        }
        pars = buf;
    }

    text.paragraphs = pars;
    text.length = len;

    return text;

    err_free_pars:
        for (int i = 0; i < len; i++)
            freepar(pars[i]);
        free(pars);
    err:
        text.length = 0;
        return text;
}

struct Paragraph *scanpar() {
    int len = 0, buf_size = 4, size_step = 8;
    struct Sentence **snts, **buf, *snt;

    snts = malloc(buf_size * sizeof(struct Sentence *));
    if (snts == NULL) {
        wprintf(L"Memory allocation error");
        goto err;
    }

    while ((snt = scansnt())) {
        if (len == buf_size) {
            if (!(buf = realloc(snts, (buf_size += size_step) * sizeof(struct Sentence *)))) {
                wprintf(L"Memory reallocation error\n");
                goto err_free_snts;
            }
            snts = buf;
        }
        snts[len++] = snt;

        if (snt->nline == true)
            break;
    }
    if (len == 0)
        goto err_free_snts;

    if (len != buf_size) {
        if (!(buf = realloc(snts, len * sizeof(struct Sentence *)))) {
            wprintf(L"Memory reallocation error\n");
            goto err_free_snts;
        }
        snts = buf;
    }

    struct Paragraph *paragraph = malloc(sizeof(struct Paragraph));

    if (paragraph == NULL) {
        wprintf(L"Memory allocation error");
        goto err_free_par;
    }

    paragraph->sentences = snts;
    paragraph->length = len;

    return paragraph;

    err_free_par:
        free(paragraph);
    err_free_snts:
        for (int i = 0; i < len; i++)
            freesnt(snts[i]);
        free(snts);
    err:
        return NULL;
}

struct Sentence *scansnt() {
    wchar_t c, *str, *buf;
    wchar_t *endchars = L".", *seps = L" ,.";
    int len = 0, buf_size = 10, size_step = 20;
    int endlcnt = 0;
    bool state = false, nline = false;

    str = malloc(buf_size * sizeof(wchar_t));
    if (str == NULL) {
        wprintf(L"Memory allocation error");
        return NULL;
    }

    while ((c = (wchar_t) getch())) {
        if (state == false) {
            if (c == '\n') {
                endlcnt++;
                if (endlcnt >= 2)
                    goto err_free_str;
            }
            if (isspace(c) || (ispunct(c) && wcschr(seps, c)))
                continue;
            state = true;
        }

        if (len == buf_size - 1) {
            if (!(buf = realloc(str, (buf_size += size_step) * sizeof(wchar_t)))) {
                wprintf(L"Memory reallocation error");
                goto err_free_str;
            }
            str = buf;
        }
        str[len++] = c;

        if (wcschr(endchars, c)) {
            c = (wchar_t) getch();
            nline = c == L'\n' ? true : false;
            ungetch(c);
            break;
        }
    }
    if (state == false)
        goto err_free_str;

    if (buf_size != len + 1) {
        if (!(buf = realloc(str, (len + 1) * sizeof(wchar_t)))) {
            wprintf(L"Memory reallocation error");
            goto err_free_str;
        }
        str = buf;
    }
    str[len] = '\0';

    struct Sentence *snt = malloc(sizeof(struct Sentence));

    if (snt == NULL) {
        wprintf(L"Memory allocation error");
        goto err_free_snt;
    }

    snt->value = str;
    snt->length = len - 1;
    snt->nline = nline;
    return snt;

    err_free_snt:
        free(snt);
    err_free_str:
        free(str);
        return NULL;
}

void printtxt(struct Text text) {
    for (int i = 0; i < text.length; i++)
        printpar(text.paragraphs[i]);
    wprintf(L"\n");
}

void printpar(struct Paragraph *paragraph) {
    for (int i = 0; i < paragraph->length; i++)
        printsnt(paragraph->sentences[i]);
    wprintf(L"\n");
}

void printsnt(struct Sentence *sentence) {
    wprintf(L"%ls ", sentence->value);
}

