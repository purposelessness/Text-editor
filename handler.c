#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "handler.h"
#include "datautility.h"
#include "memutilility.h"
#include "colorutility.h"
#include "iodata.h"
#include "dictionary.h"
#include "utility.h"

int filter_text(struct Text *text) {
    struct Paragraph **pars = text->paragraphs, *par;
    struct Sentence **snts, **buf, *snt;
    int sntcnt, parcnt = text->length;
    wchar_t *lstr;

    struct Hashtable *hashtable = create_hashtable(10);
    if (!hashtable)
        return 1;
    struct Item *item = NULL;

    for (int j = 0; j < parcnt; j++) {
        par = pars[j];
        sntcnt = par->length;
        snts = par->sentences;

        int i = 0;
        while (i < sntcnt) {
            snt = snts[i];

            item = NULL;
            lstr = wcslower(snt->value);
            item = find(hashtable, lstr);
            if (item != NULL) {
                freesnt(snt);
                free(lstr);
                if (i != sntcnt-- - 1)
                    memmove(snts + i, snts + i + 1, (sntcnt - i) * sizeof(struct Sentence *));
                continue;
            }

            add(&hashtable, lstr, snt, sizeof(*snt));
            free(lstr);
            if (!hashtable)
                goto exit;
            i++;
        }

        par->length = sntcnt;
        if (sntcnt == 0) {
            freepar(par);
            if (j != parcnt-- - 1)
                memmove(pars + j, pars + j + 1, (parcnt - j) * sizeof(struct Paragraph *));
            j--;
            continue;
        }

        if (!(buf = realloc(snts, sntcnt * sizeof(struct Sentence *))))
            wprintf(L"Memory reallocation error\n");
        else
            par->sentences = buf;
    }

    text->length = parcnt;
    if (!(pars = realloc(text->paragraphs, parcnt * sizeof(struct Paragraph *))))
        wprintf(L"Memory reallocation error\n");
    else
        text->paragraphs = pars;
    free_hashtable(hashtable);
    return 0;

    exit:
    free_hashtable(hashtable);
    return 1;
}

void colorize_text(struct Text text) {
    struct Text coloredtxt = txtcolor(text);
    if (coloredtxt.length == 0)
        return;

    printtxt(coloredtxt);
    freetxt(coloredtxt);
}

int wrdsqsort(const void *a, const void *b) {
    struct Word x = *(struct Word *) a;
    struct Word y = *(struct Word *) b;
    return (int) (wcslen(x.value) - wcslen(y.value));
}

int wrdsbsearch(const void *a, const void *b) {
    wchar_t *strwrd = (wchar_t *) a;
    struct Word wrd = *(struct Word *) b;
    return (int) (wcslen(strwrd) - wcslen(wrd.value));
}

void print_capitalized_words(struct Text text) {
    struct Word *wrds, *fwrds;
    struct Paragraph *par;
    struct Sentence **snts;
    struct Words *srcwrds;
    wchar_t **strwrds, *tmp = NULL;
    int sntcnt = 0, buf_size = 40, size_step = 25, fbuf_size = 10, fsize_step = 5;
    int wrdslen = 0, fwrdslen = 0, wrdlen, wrdsntlen_step = 5;

    if (!(wrds = malloc(buf_size * sizeof(struct Word)))) {
        wprintf(L"Memory allocation error");
        return;
    }

    if (!(fwrds = malloc(fbuf_size * sizeof(struct Word)))) {
        wprintf(L"Memory allocation error");
        free(wrds);
        return;
    }

    for (int j = 0; j < text.length; j++) {
        par = text.paragraphs[j];
        snts = par->sentences;

        for (int i = 0; i < par->length; i++) {
            srcwrds = sntwrds(*snts[i]);
            strwrds = srcwrds->value;

            for (int k = 0; k < srcwrds->length; k++) {
                wchar_t strwrd[wcslen(strwrds[k])];
                wcscpy(strwrd, strwrds[k]);
                if (wrdslen > 1) {
                    struct Word nwrd;
                    nwrd.value = strwrd;
                    nwrd.sntlen = wrdsntlen_step;
                    nwrd.sentences = malloc(nwrd.sntlen * sizeof(int));
                    continue;
                }

                tmp = NULL;
                qsort(wrds, wrdslen, sizeof(struct Word), wrdsqsort);
                tmp = bsearch(strwrd, wrds, wrdslen, sizeof(struct Word), wrdsbsearch);

                wrdlen = (int) wcslen(strwrds[k]);
            }

            free(srcwrds);
            sntcnt++;
        }
    }
}

int lastwrdcmp(const void *a, const void *b) {
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

    qsort(snts, len, sizeof(struct Sentence *), lastwrdcmp);

    for (int i = 0; i < len; i++) {
        printsnt(snts[i]);
    }
    wprintf(L"\n");
}
