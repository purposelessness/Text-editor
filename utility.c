#include "utility.h"
#include <malloc.h>
#include <wchar.h>

#define BUF_SIZE 10

wchar_t bufc[BUF_SIZE];
int bufp = 0;

wchar_t getch() {
    return bufp > 0 ? bufc[--bufp] : (wchar_t) getwchar();
}

int ungetch(wchar_t c) {
    if (bufp >= BUF_SIZE)
        return 0;
    bufc[bufp++] = (wchar_t) c;
    return 1;
}

void freetxt(struct Text text) {
    for (int i = 0; i < text.length; i++)
        freepar(text.paragraphs[i]);
    free(text.paragraphs);
}

void freepar(struct Paragraph *paragraph) {
    for (int i = 0; i < paragraph->length; i++)
        freesnt(paragraph->sentences[i]);
    free(paragraph->sentences);
    free(paragraph);
}

void freesnt(struct Sentence *sentence) {
    free(sentence->value);
    free(sentence);
}

int snticmp(const struct Sentence *snt1, const struct Sentence *snt2) {
#ifdef __linux__
    return wcsncasecmp(snt1->value, snt2->value, max((int) snt1->length, (int) snt2->length));
#elif _WIN32
    return _wcsnicmp(snt1->value, snt2->value, max((int) snt1->length, (int) snt2->length));
#endif
}

int max(int a, int b) {
    return a > b ? a : b;
}

struct Words sntwrds(struct Sentence sentence) {
    struct Words words;
    int len = 0, buf_size = 5, buf_step = 10;
    wchar_t *snt = sentence.value;
    wchar_t **wrds, **buf, *word, *pt;
    wchar_t *sep = L" ,.\n";

    wrds = malloc(buf_size * sizeof(wchar_t *));
    if (wrds == NULL) {
        wprintf(L"Memory allocation error");
        goto err;
    }

    word = wcstok(snt, sep, &pt);
    while (word != NULL) {
        if (len == buf_size) {
            if (!(buf = realloc(wrds, (buf_size += buf_step) * sizeof(wchar_t *)))) {
                wprintf(L"Memory reallocation error");
                goto err_free_wrds;
            }
            wrds = buf;
        }
        wrds[len++] = word;

        word = wcstok(NULL, sep, &pt);
    }

    if (buf_size != len) {
        if (!(buf = realloc(wrds, len * sizeof(wchar_t *)))) {
            wprintf(L"Memory reallocation error");
            goto err_free_wrds;
        }
        wrds = buf;
    }

    words.value = wrds;
    words.length = len;

    return words;

    err_free_wrds:
        for (int i = 0; i < len; i++)
            free(wrds[i]);
        free(wrds);
    err:
        words.length = 0;
        return words;
}
