#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "handler.h"
#include "datautility.h"
#include "memutilility.h"
#include "colorutility.h"

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

void color_text(struct Text src) {
    struct Sentence *snt = src.paragraphs[0]->sentences[0];
    wchar_t *str = strcolor(snt);
    wprintf(L"%ls\n", str);
    free(str);

//    free(wrds->value);
}

//void print_capitalized_text(struct Text text) {
//    struct Words words;
//
//    for (int i = 0; i < text.length; i++) {
//        words = sntwrds(*text.sentences[i]);
//
//        for (int j = 0; j < words.length; j++) {
//
//        }
//    }
//}

int cmp(const void *a, const void *b) {
    return 0;
}

void sort(struct Text text) {
    int len = 0, c = 0;
    for (int i = 0; i < text.length; i++) {
        len += text.paragraphs[i]->length;
    }
    wchar_t *snts[len];

    struct Paragraph *par;
    for (int i = 0; i < text.length; i++) {
        par = text.paragraphs[i];
        for (int j = 0; j < par->length; j++) {
            snts[c++] = par->sentences[j]->value;
        }
    }

//    qsort();
}
