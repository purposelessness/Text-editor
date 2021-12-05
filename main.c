#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <string.h>
#include "datatypes.h"
#include "iodata.h"
#include "handler.h"
#include "memutilility.h"
#include "dictionary.h"

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

    struct Hashtable *hashtable = create_hashtable(101);

    struct Sentence *snt = malloc(sizeof(struct Sentence));
    snt->value = L"Helen";
    struct Sentence *snt2 = malloc(sizeof(struct Sentence));
    snt2->value = L"Cause";
    struct Sentence *snt3 = malloc(sizeof(struct Sentence));
    snt3->value = L"Cause";

    add(hashtable, L"Hel", snt, sizeof(*snt));
    add(hashtable, L"Hel", snt, sizeof(*snt));
    add(hashtable, L"Hel", snt2, sizeof(*snt));
    add(hashtable, L"Cau", snt2, sizeof(*snt));
    add(hashtable, L"Cau", snt3, sizeof(*snt));

    struct Item *nd = find(hashtable, L"Hel");
    struct Item *nd2 = find(hashtable, L"Cau");
    wprintf(L"\n%ls\n", ((struct Sentence *)nd->value)->value);
    wprintf(L"\n%ls\n", ((struct Sentence *)nd2->value)->value);

    free_hashtable(hashtable);
    free(snt);
    free(snt2);
    free(snt3);

    return 0;
}
