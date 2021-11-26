#include "iodata.h"
#include "datatypes.h"
#include "utility.h"
#include <malloc.h>
#include <ctype.h>
#include <string.h>

struct Text *scanText() {
    size_t len = 0, buf_size = 3, size_step = 6;
    struct Sentence **sentences, **buf, *sentence;
    sentences = malloc(buf_size * sizeof(struct Sentence *));
    while ((sentence = scanSentence())) {
        if (len == buf_size - 1) {
            if ((buf = realloc(sentences, (buf_size += size_step) * sizeof(struct Sentence *))) == NULL) {
                puts("Memory reallocation error");
                goto exit;
            }
            sentences = buf;
        }
        sentences[len++] = sentence;
    }
    if (len == 0)
        goto exit;

    struct Text *text = malloc(sizeof(struct Text));
    text->sentences = sentences;
    text->length = len;
    return text;

    exit:
    {
        for (int i = 0; i < len; i++)
            freeSentence(sentences[i]);
        free(sentences);
        return NULL;
    }
}

struct Sentence *scanSentence() {
    char c, *str, *buf;
    char *endSymbols = ".", *separators = " ,.";
    size_t len = 0, buf_size = 10, size_step = 20;
    str = malloc(buf_size * sizeof(char));
    char state = 0, endlCount = 0;
    while ((c = (char) getch())) {
        if (state == 0) {
            if (c == '\n') {
                endlCount++;
                if (endlCount >= 2)
                    goto exit;
            }
            if (isspace(c) || (ispunct(c) && strchr(separators, c)))
                continue;
            state = 1;
        }

        if (len == buf_size - 1) {
            if ((buf = realloc(str, (buf_size += size_step) * sizeof(char))) == NULL) {
                puts("Memory reallocation error");
                goto exit;
            }
            str = buf;
        }
        str[len++] = c;

        if (strchr(endSymbols, c)) {
            c = (char) getch();
            str[len++] = c == '\n' ? '\n' : ' ';
            ungetch(c);
            break;
        }
    }
    if (state == 0)
        goto exit;

    if ((buf = realloc(str, (len + 1) * sizeof(char))) == NULL) {
        puts("Memory reallocation error");
        goto exit;
    }
    str = buf;
    str[len] = '\0';

    struct Sentence *sentence = malloc(sizeof(struct Sentence));
    sentence->value = str;
    return sentence;

    exit:
    {
        free(str);
        return NULL;
    }
}
