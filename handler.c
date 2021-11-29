#include <stdlib.h>
#include <string.h>
#include "handler.h"
#include "utility.h"

int sntncscmp(const void *p1, const void *p2) {
    return snticmp(*(const struct Sentence **) p1, *(const struct Sentence **) p2);
}

int cmp(const void *p1, const void *p2) {
    return snticmp((const struct Sentence *) p1, *(const struct Sentence **) p2);
}

void filter_text(struct Text *restrict text) {
    size_t txtlen = text->length;
    if (txtlen < 2)
        return;

    size_t len = 0, tlen = txtlen;
    struct Sentence **src = text->sentences;
    struct Sentence **fsnts, **tmp, *snt;

    fsnts = malloc(tlen * sizeof(struct Sentence *));
    fsnts[len++] = src[0];
    for (int i = 1; i < txtlen; i++) {
        snt = src[i];
        tmp = NULL;
        qsort(fsnts, len, sizeof(struct Sentence *), sntncscmp);
        tmp = bsearch(snt, fsnts, len, sizeof(struct Sentence *), cmp);

        if (tmp != NULL) {
            sntfree(src[i]);
            if (i != txtlen - 1) {
                memmove(src + i, src + i + 1, (txtlen-- - i - 1) * sizeof(struct Sentence *));
                i--;
            }
        } else {
            fsnts[len++] = snt;
        }
    }

    free(fsnts);
    text->length = len;
}

void color_text(struct Text text) {

}

void print_capitalized_text(struct Text text) {
    struct Words words;

    for (int i = 0; i < text.length; i++) {
        words = sntwrds(*text.sentences[i]);

        for (int j = 0; j < words.length; j++) {

        }
    }
}
