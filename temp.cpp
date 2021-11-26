#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int getText(char ***text);

int removeDeviant(char **rawText, int n, char ***text);

void printText(char **text, int n);

void freeMemory(char **text, int n);

void copySentence(char *from, char **to) {
    (*to) = calloc((int) strlen(from) + 1, sizeof(char));
    for (int i = 0; ((*to)[i] = from[i]); i++);
}

int main() {
    char **rawText;
    char **text;

    int n = getText(&rawText);
    if (n == -1)
        return -1;
    int m = removeDeviant(rawText, n, &text);
    if (m == -1)
        return -1;

    printText(text, m);
    printf("Количество предложений до %d и количество предложений после %d\n", n - 1, m - 1);

    freeMemory(text, m);
    freeMemory(rawText, n);
}

int getText(char ***text) {
    int n = 0, m = 0;
    (*text) = calloc(n + 1, sizeof(char *));
    (*text)[n] = calloc(m + 1, sizeof(char));
    int init = 1;
    char **t, *y;
    char c;
    char *end = "Dragon flew away!";

    while ((c = (char) getchar())) {
        if (isspace(c) && init == 1)
            continue;
        init = 0;
        (*text)[n][m++] = c;
        y = realloc((*text)[n], (m + 1) * sizeof(char));
        if (y == NULL)
            return -1;
        (*text)[n] = y;
        if (!(c == '.' || c == ';' || c == '?' || c == '!'))
            continue;
        (*text)[n][m] = '\0';
        m = 0;
        init = 1;
        t = realloc((*text), (++n + 1) * sizeof(char *));
        if (t == NULL)
            return -1;
        (*text) = t;
        (*text)[n] = calloc(m + 1, sizeof(char));
        if (strcmp((*text)[n - 1], end) == 0) {
            break;
        }
    }
    return n;
}

int removeDeviant(char **rawText, int n, char ***text) {
    int m = 0;
    char **t;
    (*text) = calloc(m, sizeof(char *));
    for (int i = 0; i < n; i++) {
        int flag = 1;
        char *sentence = *rawText++;
        int len = (int) strlen(sentence);
        for (int y = 0; y < len; y++) {
            char *k = sentence + y;
            if (!(*k == '5' && *(k + 1) == '5' && *(k + 2) == '5' && y < len - 2))
                continue;
            if ((y == 0 && isspace(*(k + 3))) || (isspace(*(k - 1)) && (isspace(*(k + 3)) || ispunct(*(k + 3)))))
                flag = 0;
        }
        if (flag) {
            t = realloc((*text), ++m * sizeof(char *));
            if (t == NULL)
                return -1;
            (*text) = t;
            copySentence(sentence, &((*text)[m - 1]));
        }
    }
    return m;
}

void printText(char **text, int n) {
    for (int i = 0; i < n; i++)
        puts(text[i]);
}

void freeMemory(char **text, int n) {
    for (int i = 0; i < n; i++) {
        free(text[i]);
    }
    free(text);
}
