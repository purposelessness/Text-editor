#include "iodata.h"
#include "datatypes.h"
#include "utility.h"
#include <malloc.h>
#include <ctype.h>
#include <string.h>

//struct Text *scanText() {
//    struct Sentence *buf, *sentence, **sentences;
//    char *endSymbols = ".", *separators = " ,.";
//    size_t len = 0, buf_size = 3, size_step = 6;
//    struct Text *text = malloc(sizeof(struct Text));
//    *sentences = malloc(buf_size * sizeof(struct Sentence *));
//    while ((sentence = scanSentence())) {
//        if (len == buf_size) {
//            buf = *sentences;
//            if ((*str = realloc(*str, (buf_size += size_step) * sizeof(char))) == NULL) {
//                free(buf);
//                puts("Memory reallocation error");
//            }
//        }
//        (*str)[len++] = c;
//
//        if (strchr(endSymbols, c)) {
//            c = (char) getch();
//            if (c == '\n')
//                (*str)[len++] = c;
//            ungetch(c);
//            break;
//        }
//    }
//    if (state == 0)
//        goto exit;
//
//    buf = *str;
//    if ((*str = realloc(*str, (len + 1) * sizeof(char))) == NULL) {
//        free(buf);
//        puts("Memory reallocation error");
//        goto exit;
//    }
//    (*str)[len] = '\0';
//
//    sentence->value = *str;
//    return sentence;
//
//    exit:
//    {
//        free(*str);
//        free(sentence);
//        return NULL;
//    }
//}

struct Sentence *scanSentence() {
    char c, *buf, **str;
    char *endSymbols = ".", *separators = " ,.";
    size_t len = 0, buf_size = 10, size_step = 20;
    struct Sentence *sentence = malloc(sizeof(struct Sentence));
    str = &(sentence->value);
    *str = malloc(buf_size * sizeof(char));
    char state = 0, endlCount = 0;
    while ((c = (char) getch())) {
        if (state == 0) {
            if (c == '\n') {
                endlCount++;
                if (endlCount >= 2)
                    goto exit;
            }
            if (isspace(c) || (ispunct(c) && strchr(separators, c)))
                continue;
            state = 1;
        }

        if (len == buf_size - 1) {
            buf = *str;
            if ((*str = realloc(*str, (buf_size += size_step) * sizeof(char))) == NULL) {
                free(buf);
                puts("Memory reallocation error");
            }
        }
        (*str)[len++] = c;

        if (strchr(endSymbols, c)) {
            c = (char) getch();
            if (c == '\n')
                (*str)[len++] = c;
            ungetch(c);
            break;
        }
    }
    if (state == 0)
        goto exit;

    buf = *str;
    if ((*str = realloc(*str, (len + 1) * sizeof(char))) == NULL) {
        free(buf);
        puts("Memory reallocation error");
        goto exit;
    }
    (*str)[len] = '\0';

    sentence->value = *str;
    return sentence;

    exit:
    {
        free(*str);
        free(sentence);
        return NULL;
    }
}
