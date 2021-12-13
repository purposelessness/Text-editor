#include "iodata.h"
#include "ioutility.h"
#include "memutilility.h"
#include <malloc.h>
#include <ctype.h>
#include <wchar.h>

struct Text scan_text() {
    struct Text text;
    int len = 0, buf_size = 3, size_step = 2;
    struct Paragraph **pars, **buf, *par;

    pars = malloc(buf_size * sizeof(struct Paragraph *));
    if (pars == NULL) {
        wprintf(L"Memory allocation error\n");
        goto err;
    }

    while ((par = scan_paragraph())) {
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
        free_paragraph(pars[i]);
    free(pars);
    err:
    text.length = 0;
    return text;
}

struct Paragraph *scan_paragraph() {
    int len = 0, buf_size = 8, sizestep = 5;
    struct Sentence **snts, **buf, *snt;

    snts = malloc(buf_size * sizeof(struct Sentence *));
    if (snts == NULL) {
        wprintf(L"Memory allocation error\n");
        goto err;
    }

    while ((snt = scan_sentence())) {
        if (len == buf_size) {
            if (!(buf = realloc(snts, (buf_size += sizestep) * sizeof(struct Sentence *)))) {
                wprintf(L"Memory reallocation error\n");
                goto err_free_snts;
            }
            snts = buf;
        }
        snts[len++] = snt;
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
        wprintf(L"Memory allocation error\n");
        goto err_free_par;
    }

    paragraph->sentences = snts;
    paragraph->length = len;

    return paragraph;

    err_free_par:
    free(paragraph);
    err_free_snts:
    for (int i = 0; i < len; i++)
        free_sentence(snts[i]);
    free(snts);
    err:
    return NULL;
}

struct Sentence *scan_sentence() {
    struct Sentence *snt;
    wchar_t c, *str, *buf;
    wchar_t *endchars = L".", *seps = L" ,.";
    int len = 0, bufsize = 60, sizestep = 30;
    bool state = false;

    snt = malloc(sizeof(struct Sentence));
    if (snt == NULL) {
        wprintf(L"Memory allocation error\n");
        return NULL;
    }

    str = malloc(bufsize * sizeof(wchar_t));
    if (str == NULL) {
        wprintf(L"Memory allocation error\n");
        goto err_free_snt;
    }

    while ((c = (wchar_t) getch())) {
        if (state == false) {
            if (c == '\n')
                goto err_free_str;
            if (isspace(c) || wcschr(seps, c))
                continue;
            state = true;
        }

        if (len == bufsize - 1) {
            if (!(buf = realloc(str, (bufsize += sizestep) * sizeof(wchar_t)))) {
                wprintf(L"Memory reallocation error\n");
                goto err_free_str;
            }
            str = buf;
        }
        str[len++] = c;

        if (wcschr(endchars, c))
            break;
    }
    if (len == 0 || state == false)
        goto err_free_str;

    if (bufsize != len + 1) {
        if (!(buf = realloc(str, (len + 1) * sizeof(wchar_t)))) {
            wprintf(L"Memory reallocation error\n");
            goto err_free_str;
        }
        str = buf;
    }
    str[len++] = '\0';

    snt->value = str;
    snt->length = len;
    return snt;

    err_free_str:
    free(str);
    err_free_snt:
    free(snt);
    return NULL;
}

void print_text(struct Text text) {
    for (int i = 0; i < text.length; i++)
        print_paragraph(text.paragraphs[i]);
    wprintf(L"\n");
}

void print_paragraph(struct Paragraph *paragraph) {
    for (int i = 0; i < paragraph->length; i++)
        print_sentence(paragraph->sentences[i]);
    wprintf(L"\n");
}

void print_sentence(struct Sentence *sentence) {
    wprintf(L"%ls ", sentence->value);
}

