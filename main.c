#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <string.h>
#include "datatypes.h"
#include "iodata.h"
#include "handler.h"
#include "memutilility.h"
#include "dictionary.h"

void somefunc(int **arr) {
    int *narr = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
        narr[i] = i + 2;
    free(*arr);
    *arr = narr;
}

int main() {
    setlocale(LC_CTYPE, "");

//    struct Text text = scantxt();
//    if (text.length == 0)
//        return 0;
//
//    printtxt(text);
//
//    int res = filter_text(&text);
//    if (res != 0) {
//        wprintf(L"Text filter error\n");
//        freetxt(text);
//        return 1;
//    }
//
//
//    colorize_text(text);
//
//    print_sorted_text(text);
//
//    freetxt(text);

    struct Hashtable *hashtable = create_hashtable(1);

    struct Sentence *snt = malloc(sizeof(struct Sentence));
    snt->value = L"snt1";
    struct Sentence *snt2 = malloc(sizeof(struct Sentence));
    snt2->value = L"snt2";
    struct Sentence *snt3 = malloc(sizeof(struct Sentence));
    snt3->value = L"snt3";

    add(&hashtable, L"Hel", snt, sizeof(*snt));
    add(&hashtable, L"Cau", snt2, sizeof(*snt));
    add(&hashtable, L"LOL", snt3, sizeof(*snt));

    struct Item *nd = find(hashtable, L"Hel");
    struct Item *nd2 = find(hashtable, L"Cau");
    struct Item *nd3 = find(hashtable, L"LOL");
    wprintf(L"\n%ls\n", ((struct Sentence *)nd->value)->value);
    wprintf(L"\n%ls\n", ((struct Sentence *)nd2->value)->value);
    wprintf(L"\n%ls\n", ((struct Sentence *)nd3->value)->value);
    wprintf(L"%d", hashtable->size);

    free_hashtable(hashtable);
    free(snt);
    free(snt2);
    free(snt3);

    return 0;
}
