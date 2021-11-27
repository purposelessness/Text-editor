#include <stdlib.h>
#include <string.h>
#include "handler.h"
#include "utility.h"

int sntncscmp(const void *p1, const void *p2) {
    return snticmp(*(const struct Sentence **)p1, *(const struct Sentence **)p2);
}

int cmp(const void *p1, const void *p2) {
    return snticmp((const struct Sentence *)p1, *(const struct Sentence **)p2);
}

void filtertxt(struct Text *restrict text) {
    size_t txtlen = text->length;
    if (txtlen < 2)
        return;

    size_t len = 0, tlen = txtlen;
    struct Sentence **src = text->sentences;
    struct Sentence **fsentences, **pSnt, *snt;

    fsentences = malloc(tlen * sizeof(struct Sentence *));
    fsentences[len++] = src[0];
    for (int i = 1; i < txtlen; i++) {
        snt = src[i];
        pSnt = NULL;
        qsort(fsentences, len, sizeof(struct Sentence *), sntncscmp);
        pSnt = bsearch(snt, fsentences, len, sizeof(struct Sentence *), cmp);

        if (pSnt != NULL) {
            sntfree(src[i]);
            if (i != txtlen - 1) {
                memmove(src + i, src + i + 1, (txtlen-- - i - 1) * sizeof(struct Sentence *));
                i--;
            }
        }
        else
            fsentences[len++] = snt;
    }

    free(fsentences);
    text->length = len;
}
