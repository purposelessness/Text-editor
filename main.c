#include <locale.h>
#include "datatypes.h"
#include "iodata.h"
#include "utility.h"

int main() {
    setlocale(LC_CTYPE, "");

    struct Text *text = scanText();
    if (text == NULL)
        return 0;
    printText(text);
    freeText(text);

    return 0;
}
