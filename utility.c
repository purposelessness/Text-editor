#include <wchar.h>
#include <malloc.h>
#include <wctype.h>
#include "utility.h"

bool isnumber(wchar_t *s) {
    bool flag = true;
    for (; *s; s++) {
        if (iswdigit(*s) == 0) {
            flag = false;
            break;
        }
    }
    return flag;
}

wchar_t *remove_duplicates(wchar_t *s) {
    wchar_t *noduplicates = malloc((wcslen(s) + 1) * sizeof(wchar_t));
    int i = 0;
    for (; *s; s++) {
        if (!wcschr(noduplicates, towlower(*s))) {
            noduplicates[i++] = towlower(*s);
        }
    }
    noduplicates[i] = '\0';
    return noduplicates;
}

wchar_t *wcslower(wchar_t *s) {
    int size = (int) wcslen(s);
    wchar_t *ls = malloc((size + 1) * sizeof(wchar_t));
    for (int i = 0; i < size; i++) {
        ls[i] = (wchar_t) towlower(s[i]);
    }
    ls[size] = '\0';
    return ls;
}
