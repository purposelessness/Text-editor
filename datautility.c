#include <wchar.h>
#include <malloc.h>
#include "datautility.h"
#include "utility.h"

int snticmp(const struct Sentence *snt1, const struct Sentence *snt2) {
#ifdef __linux__
    return wcsncasecmp(snt1->value, snt2->value, max((int) snt1->length, (int) snt2->length) - 2);
#elif _WIN32
    return _wcsnicmp(snt1->value, snt2->value, max((int) snt1->sntlen, (int) snt2->sntlen) - 2);
#endif
}

struct Words *sntwrds(const struct Sentence sentence) {
    struct Words *words;
    int len = 0, buf_size = 10, buf_step = 5;
    wchar_t *str;
    wchar_t **arr, **buf, *word, *pt;
    wchar_t *sep = L" ,.\n";

    str = malloc(sizeof(wchar_t) * sentence.length);
    if (str == NULL) {
        wprintf(L"Memory allocation error");
        return NULL;
    }
    wcsncpy(str, sentence.value, sentence.length);

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
