#include <locale.h>
#include <wchar.h>
#include "datatypes.h"
#include "iodata.h"
#include "handler.h"
#include "memutilility.h"

int main() {
    setlocale(LC_CTYPE, "");

    struct Text text = scan_text();
    if (text.length == 0)
        return 0;

    int res = filter_text(&text);
    if (res != 0) {
        wprintf(L"Text filter error\n");
        free_text(text);
        return 1;
    }

    print_colorized_text(text);

    print_capitalized_words(text);

    print_sorted_text(text);

    free_text(text);

    return 0;
}
