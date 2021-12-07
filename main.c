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

    wprintf(L"Введите команду:\n0: Выход из программы\n1: Раскрасить слова в тексте\n2: Распечатать каждое слово, которое начинается и "
            "заканчивается заглавной буквой\n3: Отсортировать предложения по длине последнего слова\n"
            "4: Удалить все числа из предложений\n\n");
    int command;
    wscanf(L"%d", &command);
    wprintf(L"\n");

    switch (command) {
        case 0:
            wprintf(L":(");
            break;
        case 1:
            print_colorized_text(text);
            break;
        case 2:
            print_capitalized_words(text);
            break;
        case 3:
            print_sorted_text(text);
            break;
        case 4:
            print_text_without_numbers(text);
            break;
        default:
            wprintf(L"Unknown command");
            break;
    }

    free_text(text);

    return 0;
}
