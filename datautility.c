#include <wchar.h>
#include <malloc.h>
#include <string.h>
#include "datautility.h"
#include "memutility.h"

struct Words *sntwrds(const struct Sentence *snt) {
    struct Words *words;
    int len = 0, buf_size = 10, buf_step = 5;
    wchar_t *str;
    wchar_t **arr, **buf, *word, *pt;
    wchar_t *sep = L" ,.\n";

    str = malloc(sizeof(wchar_t) * snt->length);
    if (str == NULL) {
        wprintf(L"Memory allocation error");
        return NULL;
    }
    wcsncpy(str, snt->value, snt->length);

    words = malloc(sizeof(struct Words));
    if (words == NULL) {
        wprintf(L"Memory allocation error\n");
        goto err_free_str;
    }

    arr = malloc(buf_size * sizeof(wchar_t *));
    if (arr == NULL) {
        wprintf(L"Memory allocation error\n");
        goto err_free_words;
    }

    word = wcstok(str, sep, &pt);
    while (word != NULL) {
        if (len == buf_size) {
            if (!(buf = realloc(arr, (buf_size += buf_step) * sizeof(wchar_t *)))) {
                wprintf(L"Memory reallocation error\n");
                goto err_free_arr;
            }
            arr = buf;
        }
        arr[len++] = word;

        word = wcstok(NULL, sep, &pt);
    }

    if (buf_size != len) {
        if (!(buf = realloc(arr, len * sizeof(wchar_t *)))) {
            wprintf(L"Memory reallocation error\n");
            goto err_free_arr;
        }
        arr = buf;
    }

    words->value = arr;
    words->length = len;

    return words;

    err_free_arr:
    free(arr);
    err_free_words:
    free(words);
    err_free_str:
    free(str);
    return NULL;
}

int txtclear(struct Text *txt) {
    int i = 0, tmptxtlen = txt->length;
    while (i < txt->length) {
        struct Paragraph *par = txt->paragraphs[i];
        int j = 0, tmpparlen = par->length;
        while (j < par->length) {
            struct Sentence *snt = par->sentences[j];
            if (snt) {
                j++;
                continue;
            }

            if (j != --par->length)
                memmove(par->sentences + j, par->sentences + j + 1, (par->length - j) * sizeof(struct Sentence *));
        }

        if (par->length == 0) {
            free_paragraph(par);
            if (i == --txt->length)
                break;
            memmove(txt->paragraphs + i, txt->paragraphs + i + 1, (txt->length - i) * sizeof(struct Paragraph *));
            continue;
        }

        if (tmpparlen != par->length) {
            struct Sentence **buf;
            if (!(buf = realloc(par->sentences, par->length * sizeof(struct Sentence *)))) {
                wprintf(L"Memory reallocation error");
                return 1;
            }
            par->sentences = buf;
        }
        i++;
    }

    if (txt->length == 0) {
        free_text(*txt);
        return 0;
    }

    if (tmptxtlen != txt->length) {
        struct Paragraph **buf;
        if (!(buf = realloc(txt->paragraphs, txt->length * sizeof(struct Paragraph *)))) {
            wprintf(L"Memory reallocation error");
            return 1;
        }
        txt->paragraphs = buf;
    }
    return 0;
}
