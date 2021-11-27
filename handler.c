#include <wchar.h>
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

int filtertxt(struct Text *restrict text) {
    size_t len = 0, buf_size = 3, size_step = 6;
    struct Sentence **src = text->sentences;
    struct Sentence **fsentences, **sentences, **buf, **pSnt, *snt;
    sentences = malloc(buf_size * sizeof(struct Sentence *));
    size_t txtlen = text->length;
    for (int i = 0; i < txtlen; i++) {
        snt = src[i];
        if (len == buf_size) {
            if (!(buf = realloc(sentences, (buf_size += size_step) * sizeof(struct Sentence *)))) {
                wprintf(L"Memory reallocation error\n");
                goto exit;
            }
            sentences = buf;
        }
        if (len > 0) {
            pSnt = NULL;
            fsentences = malloc(len * sizeof(struct Sentence *));
            memcpy(fsentences, sentences, len * sizeof(struct Sentence *));
            qsort(fsentences, len, sizeof(struct Sentence *), sntncscmp);
            pSnt = bsearch(snt, fsentences, len, sizeof(struct Sentence *), cmp);
            free(fsentences);
        }
        if (pSnt == NULL || len == 0)
            sentences[len++] = snt;
        else
            sntfree(snt);
    }
    if (len == 0)
        goto exit;

    if (buf_size > len) {
        if (!(buf = realloc(sentences, len * sizeof(struct Sentence *)))) {
            wprintf(L"Memory reallocation error\n");
            goto exit;
        }
        sentences = buf;
    }

    free(src);

    text->sentences = sentences;
    text->length = len;
    wprintf(L"len is %d\n", len);

    return 0;

    exit:
    {
        for (int i = 0; i < len; i++)
            sntfree(sentences[i]);
        free(sentences);
        return 1;
    }
}
