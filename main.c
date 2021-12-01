#include <locale.h>
#include "datatypes.h"
#include "iodata.h"
#include "utility.h"
#include "handler.h"

int main() {
    setlocale(LC_CTYPE, "");

    struct Text text = scantxt();
    if (text.length == 0)
        return 0;

    printtxt(text);

    filter_text(&text);

    printtxt(text);

    freetxt(text);

    return 0;
}
