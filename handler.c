#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "handler.h"
#include "datautility.h"
#include "memutilility.h"
#include "colorutility.h"
#include "iodata.h"

int sntncscmp(const void *p1, const void *p2) {
    return snticmp(*(const struct Sentence **) p1, *(const struct Sentence **) p2);
}

int bsearchcmp(const void *p1, const void *p2) {
    return snticmp((const struct Sentence *) p1, *(const struct Sentence **) p2);
}

void filter_text(struct Text *text) {
    struct Paragraph **pars = text->paragraphs, *par;
    struct Sentence **snts, **fsnts, **buf, **tmp, *snt;
    int sntcnt, parcnt = text->length;
    int flen = 0, buf_size = 3, size_step = 6;

    fsnts = malloc(buf_size * sizeof(struct Sentence *));

    for (int j = 0; j < parcnt; j++) {
        par = pars[j];
        sntcnt = par->length;
        snts = par->sentences;

        int i = 0;
        while (i < sntcnt) {
            snt = snts[i];

            if (flen < 1) {
                fsnts[flen++] = snt;
                i++;
                continue;
            }

            tmp = NULL;
            qsort(fsnts, flen, sizeof(struct Sentence *), sntncscmp);
            tmp = bsearch(snt, fsnts, flen, sizeof(struct Sentence *), bsearchcmp);

            if (tmp != NULL) {
                freesnt(snt);

                if (i != sntcnt-- - 1) {
                    memmove(snts + i, snts + i + 1, (sntcnt - i) * sizeof(struct Sentence *));
                }
                continue;
            }
            if (flen == buf_size) {
                if (!(buf = realloc(fsnts, (buf_size += size_step) * sizeof(struct Sentence *)))) {
                    wprintf(L"Memory reallocation error\n");
                    par->length = sntcnt;
                    goto exit;
                }
                fsnts = buf;
            }

            fsnts[flen++] = snt;
            i++;
        }

        par->length = sntcnt;
        if (sntcnt == 0) {
            freepar(par);

            if (j != parcnt-- - 1) {
                memmove(pars + j, pars + j + 1, (parcnt - j) * sizeof(struct Paragraph *));
            }
            j--;
            continue;
        }

        if (!(buf = realloc(snts, sntcnt * sizeof(struct Sentence *)))) {
            wprintf(L"Memory reallocation error\n");

        }
        par->sentences = buf;
    }

    text->length = parcnt;
    text->paragraphs = realloc(text->paragraphs, parcnt * sizeof(struct Paragraph *));

    exit:
    free(fsnts);
}

void colorize_text(struct Text text) {
    struct Text coloredtxt = txtcolor(text);
    printtxt(coloredtxt);
    freetxt(coloredtxt);
}

void print_capitalized_words(struct Text text) {
    struct Word *wrds;
}

int cmp(const void *a, const void *b) {
    struct Sentence *x = *(struct Sentence **) a;
    struct Sentence *y = *(struct Sentence **) b;
    struct Words *xw = sntwrds(*x), *yw = sntwrds(*y);
    int out = (int) (wcslen(xw->value[xw->length - 1]) - wcslen(yw->value[yw->length - 1]));
    freewrds(xw);
    freewrds(yw);
    return out;
}

void print_sorted_text(struct Text text) {
    int len = 0, c = 0;
    for (int i = 0; i < text.length; i++) {
        len += text.paragraphs[i]->length;
    }
    struct Sentence *snts[len];

    struct Paragraph *par;
    for (int i = 0; i < text.length; i++) {
        par = text.paragraphs[i];
        for (int j = 0; j < par->length; j++) {
            snts[c++] = par->sentences[j];
        }
    }

    qsort(snts, len, sizeof(struct Sentence *), cmp);

    for (int i = 0; i < len; i++) {
        printsnt(snts[i]);
    }
    wprintf(L"\n");
}
