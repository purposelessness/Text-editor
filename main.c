#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <string.h>
#include "datatypes.h"
#include "iodata.h"
#include "handler.h"
#include "memutilility.h"
#include "dictionary.h"
#include "utility.h"

void somefunc(int **arr) {
    int *narr = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
        narr[i] = i + 2;
    free(*arr);
    *arr = narr;
}

int main() {
    setlocale(LC_CTYPE, "");

    struct Text text = scantxt();
    if (text.length == 0)
        return 0;

    printtxt(text);

    int res = filter_text(&text);
    if (res != 0) {
        wprintf(L"Text filter error\n");
        freetxt(text);
        return 1;
    }


    colorize_text(text);

    print_sorted_text(text);

    freetxt(text);

    return 0;
}
