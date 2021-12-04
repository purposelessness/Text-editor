#include <locale.h>
#include "datatypes.h"
#include "iodata.h"
#include "handler.h"
#include "memutilility.h"

int main() {
    setlocale(LC_CTYPE, "");

    struct Text text = scantxt();
    if (text.length == 0)
        return 0;

    printtxt(text);

    filter_text(&text);

    colorize_text(text);

    print_sorted_text(text);

    freetxt(text);

    return 0;
}
