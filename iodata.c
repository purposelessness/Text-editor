#include "iodata.h"
#include "utility.h"
#include <malloc.h>
#include <ctype.h>
#include <wchar.h>

struct Text txtscan() {
    struct Text text;
    size_t len = 0, buf_size = 3, size_step = 6;
    struct Sentence **sentences, **buf, *sentence;
    sentences = malloc(buf_size * sizeof(struct Sentence *));
    while ((sentence = sntscan())) {
        if (len == buf_size) {
            if (!(buf = realloc(sentences, (buf_size += size_step) * sizeof(struct Sentence *)))) {
                wprintf(L"Memory reallocation error\n");
                goto exit;
            }
            sentences = buf;
        }
        sentences[len++] = sentence;
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

    text.sentences = sentences;
    text.length = len;

    return text;

    exit:
    {
        for (int i = 0; i < len; i++)
            sntfree(sentences[i]);
        free(sentences);
        text.length = 0;
        return text;
    }
}

struct Sentence *sntscan() {
    wchar_t c, *str, *buf;
    wchar_t *endSymbols = L".", *separators = L" ,.";
    size_t len = 0, buf_size = 10, size_step = 20;
    str = malloc(buf_size * sizeof(wchar_t));
    char state = 0, endlCount = 0;
    while ((c = (wchar_t) getch())) {
        if (state == 0) {
            if (c == '\n') {
                endlCount++;
                if (endlCount >= 2)
                    goto exit;
            }
            if (isspace(c) || (ispunct(c) && wcschr(separators, c)))
                continue;
            state = 1;
        }

        if (len == buf_size - 1) {
            if (!(buf = realloc(str, (buf_size += size_step) * sizeof(wchar_t)))) {
                wprintf(L"Memory reallocation error");
                goto exit;
            }
            str = buf;
        }
        str[len++] = c;

        if (wcschr(endSymbols, c)) {
            c = (wchar_t) getch();
            str[len++] = c == L'\n' ? L'\n' : L' ';
            ungetch(c);
            break;
        }
    }
    if (state == 0)
        goto exit;

    if (!(buf = realloc(str, (len + 1) * sizeof(wchar_t)))) {
        wprintf(L"Memory reallocation error");
        goto exit;
    }
    str = buf;
    str[len] = '\0';

    struct Sentence *sentence = malloc(sizeof(struct Sentence));
    sentence->value = str;
    sentence->length = len - 2;
    return sentence;

    exit:
    {
        free(str);
        return NULL;
    }
}

void txtprint(struct Text text) {
    for (int i = 0; i < text.length; i++)
        sntprint(text.sentences[i]);
}

void sntprint(struct Sentence *restrict sentence) {
    wprintf(sentence->value);
}