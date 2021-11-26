#include <wchar.h>
#include <locale.h>
#include "datatypes.h"
#include "utility.h"
#include "iodata.h"

int main() {
    setlocale(LC_CTYPE, "");

    struct Text *text = scanText();
    if (text == NULL)
        return 0;

    for (int i = 0; i < text->length; i++)
        wprintf(L"%ls", text->sentences[i]->value);
    freeText(text);

    return 0;
}
