#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "handler.h"
#include "utility.h"

int sntncscmp(const void *p1, const void *p2) {
    return snticmp(*(const struct Sentence **) p1, *(const struct Sentence **) p2);
}

int cmp(const void *p1, const void *p2) {
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

            if (flen < 2) {
                fsnts[flen++] = snt;
                i++;
                continue;
            }

            tmp = NULL;
            qsort(fsnts, flen, sizeof(struct Sentence *), sntncscmp);
            tmp = bsearch(snt, fsnts, flen, sizeof(struct Sentence *), cmp);

            if (tmp != NULL) {
                freesnt(snt);

                if (i != sntcnt-- - 1) {
                    memmove(snts + i, snts + i + 1, (sntcnt - i) * sizeof(struct Sentence *));
                }
                continue;
            }
            if (flen == buf_size) {
                if (!(buf = realloc(fsnts, (buf_size += size_step) * sizeof(struct Sentence *)))) {
                    wprintf(L"Memory reallocation error");
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
        }
    }

    text->length = parcnt;
    exit:
    free(fsnts);
}

void color_text(struct Text text) {

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
