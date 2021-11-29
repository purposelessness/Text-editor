#include "iodata.h"
#include "utility.h"
#include <malloc.h>
#include <ctype.h>
#include <wchar.h>

struct Text txtscan() {
    struct Text text;
    size_t len = 0, buf_size = 3, size_step = 6;
    struct Sentence **snts, **buf, *snt;

    snts = malloc(buf_size * sizeof(struct Sentence *));
    if (snts == NULL) {
        wprintf(L"Memory allocation error");
        goto err;
    }

    while ((snt = sntscan())) {
        if (len == buf_size) {
            if (!(buf = realloc(snts, (buf_size += size_step) * sizeof(struct Sentence *)))) {
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

    text.sentences = snts;
    text.length = len;

    return text;

    err_free_snts:
        for (int i = 0; i < len; i++)
            sntfree(snts[i]);
        free(snts);
    err:
        text.length = 0;
        return text;
}

struct Sentence *sntscan() {
    wchar_t c, *str, *buf;
    wchar_t *endchars = L".", *seps = L" ,.";
    size_t len = 0, buf_size = 10, size_step = 20;
    char state = 0, endlcnt = 0;

    str = malloc(buf_size * sizeof(wchar_t));
    if (str == NULL) {
        wprintf(L"Memory allocation error");
        return NULL;
    }

    while ((c = (wchar_t) getch())) {
        if (state == 0) {
            if (c == '\n') {
                endlcnt++;
                if (endlcnt >= 2)
                    goto err_free_str;
            }
            if (isspace(c) || (ispunct(c) && wcschr(seps, c)))
                continue;
            state = 1;
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
            str[len++] = c == L'\n' ? L'\n' : L' ';
            ungetch(c);
            break;
        }
    }
    if (state == 0)
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
    snt->length = len - 2;
    return snt;

    err_free_snt:
        free(snt);
    err_free_str:
        free(str);
        return NULL;
}

void txtprint(struct Text text) {
    for (int i = 0; i < text.length; i++)
        sntprint(text.sentences[i]);
    wprintf(L"\n");
}

void sntprint(struct Sentence *restrict sentence) {
    wprintf(sentence->value);
}