#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include "handler.h"
#include "datautility.h"
#include "memutility.h"
#include "colorutility.h"
#include "iodata.h"
#include "hashtable.h"
#include "utility.h"
#include "rmnumsutility.h"

int filter_text(struct Text *text) {
    struct Hashtable *hashtable = create_hashtable(10);
    if (!hashtable)
        return 1;

    for (int j = 0; j < text->length; j++) {
        struct Paragraph *par = text->paragraphs[j];
        for (int i = 0; i < par->length; i++) {
            struct Sentence *snt = par->sentences[i];
            wchar_t *lstr = wcslower(snt->value);
            void *item = find(hashtable, lstr);

            if (item) {
                free_sentence(snt);
                free(lstr);
                par->sentences[i] = NULL;
                continue;
            }

            add(&hashtable, lstr, snt, sizeof(*snt));
            free(lstr);
            if (!hashtable)
                return 1;
        }
    }
    free_hashtable(hashtable);
    if (txtclear(text) != 0 || text->length == 0)
        return 1;
    return 0;
}

void print_colorized_text(const struct Text *text) {
    struct Text coloredtxt = txtcolor(text);
    if (coloredtxt.length == 0)
        return;

    print_text(coloredtxt);
    free_text(coloredtxt);
}

void print_capitalized_words(const struct Text *text) {
    int sntcnt = 1;
    struct Hashtable *hashtable = create_hashtable(50);
    if (!hashtable)
        return;

    for (int j = 0; j < text->length; j++) {
        struct Paragraph *par = text->paragraphs[j];
        for (int i = 0; i < par->length; i++) {
            struct Words *srcwrds = sntwrds(par->sentences[i]);
            if (!srcwrds)
                goto err_free_hashtable;
            wchar_t **strwrds = srcwrds->value;

            for (int k = 0; k < srcwrds->length; k++) {
                wchar_t *rawstr = strwrds[k];
                bool flag = iswupper(rawstr[0]) && iswupper(rawstr[wcslen(rawstr) - 1]) ? true : false;
                wchar_t *strwrd = wcslower(rawstr);

                struct Item *item = find(hashtable, strwrd);
                if (item != NULL) {
                    struct Word *wrd = (struct Word *) item;
                    wrd->flag = flag ? true : wrd->flag;
                    if (wrd->length >= wrd->size) {
                        int *buf;
                        if (!(buf = realloc(wrd->sentences, (wrd->size *= 2) * sizeof(int)))) {
                            wprintf(L"Memory reallocation error");
                            free_words(srcwrds);
                            goto err_free_hashtable;
                        }
                        wrd->sentences = buf;
                    }
                    wrd->sentences[wrd->length++] = flag == true ? -sntcnt : sntcnt;
                    free(strwrd);
                    continue;
                }

                struct Word wrd;
                wrd.value = strwrd;
                wrd.size = 5, wrd.length = 0;
                wrd.flag = flag;
                wrd.sentences = malloc(wrd.size * sizeof(int));
                if (!wrd.sentences) {
                    wprintf(L"Memory allocation error");
                    free(strwrd);
                    goto err_free_hashtable;
                }
                wrd.sentences[wrd.length++] = flag == true ? -sntcnt : sntcnt;
                add(&hashtable, strwrd, &wrd, sizeof(wrd));
                free(strwrd);
                if (!hashtable) {
                    free_words(srcwrds);
                    return;
                }
            }

            free_words(srcwrds);
            sntcnt++;
        }
    }

    bool found = false;
    for (int i = 0; i < hashtable->size; i++) {
        struct Item *item = hashtable->items[i];
        if (!item)
            continue;
        struct Word *wrd = (struct Word *) item->value;
        if (wrd->flag == false)
            continue;
        found = true;
        wprintf(L"%ls: ", item->key);
        for (int y = 0; y < wrd->length; y++) {
            int n = wrd->sentences[y];
            if (n < 0)
                wprintf(L"\033[34m%d\033[0m ", -n);
            else
                wprintf(L"%d ", n);
        }
        wprintf(L"\n");
    }

    if (found == false)
        wprintf(L"The words needed not found\n");
    wprintf(L"\n");

    err_free_hashtable:
    for (int i = 0; i < hashtable->size; i++) {
        struct Item *item = hashtable->items[i];
        if (!item)
            continue;
        struct Word *wrd = (struct Word *) item->value;
        if (wrd->sentences)
            free(wrd->sentences);
    }
    free_hashtable(hashtable);
}

int lastwrdcmp(const void *a, const void *b) {
    struct Sentence *x = *(struct Sentence **) a;
    struct Sentence *y = *(struct Sentence **) b;
    struct Words *xw = sntwrds(x), *yw = sntwrds(y);
    if (!xw || !yw)
        return 0;
    int out = (int) (wcslen(xw->value[xw->length - 1]) - wcslen(yw->value[yw->length - 1]));
    free_words(xw);
    free_words(yw);
    return out;
}

void print_sorted_text(const struct Text *text) {
    int len = 0, c = 0;
    for (int i = 0; i < text->length; i++) {
        len += text->paragraphs[i]->length;
    }
    struct Sentence *snts[len];

    struct Paragraph *par;
    for (int i = 0; i < text->length; i++) {
        par = text->paragraphs[i];
        for (int j = 0; j < par->length; j++) {
            snts[c++] = par->sentences[j];
        }
    }

    qsort(snts, len, sizeof(struct Sentence *), lastwrdcmp);

    for (int i = 0; i < len; i++) {
        print_sentence(snts[i]);
    }
    wprintf(L"\n\n");
}

struct Text txtnonums(struct Text *text) {
    struct Text txt = txtrmnums(text);
    txtclear(&txt);
    return txt;
}

void print_text_without_numbers(struct Text *text) {
    struct Text txt = txtnonums(text);
    if (txt.length == 0)
        return;
    print_text(txt);
    free_text(txt);
}

int wrdcmptask(const void *a, const void *b) {
    wchar_t x = *(wchar_t *) a;
    wchar_t y = *(wchar_t *) b;
    return x - y;
}

int sntcmptask(const void *a, const void *b) {
    struct Sentence *x = *(struct Sentence **) a;
    struct Sentence *y = *(struct Sentence **) b;
    struct Words *wx = sntwrds(x);
    struct Words *wy = sntwrds(y);
    int c1 = 0, c2 = 0;
    struct Hashtable *hashtable1 = create_hashtable(20);
    struct Hashtable *hashtable2 = create_hashtable(20);
    for (int i = 0; i < wx->length; i++) {
        wchar_t *nodupl = remove_duplicates(wx->value[i]);
        qsort(nodupl, wcslen(nodupl), sizeof(wchar_t), wrdcmptask);
        struct Item *item = find(hashtable1, nodupl);
        if (item) {
            c1++;
            free(nodupl);
            continue;
        }
        add(&hashtable1, nodupl, nodupl, sizeof(wchar_t *));
        free(nodupl);
    }
    for (int i = 0; i < wy->length; i++) {
        wchar_t *nodupl = remove_duplicates(wy->value[i]);
        qsort(nodupl, wcslen(nodupl), sizeof(wchar_t), wrdcmptask);
        struct Item *item = find(hashtable2, nodupl);
        if (item) {
            c2++;
            free(nodupl);
            continue;
        }
        add(&hashtable2, nodupl, nodupl, sizeof(wchar_t *));
        free(nodupl);
    }
    free_words(wx);
    free_words(wy);
    free_hashtable(hashtable1);
    free_hashtable(hashtable2);
    return c1 - c2;
}

void sort_text_task(struct Text *text) {
    int len = 0;
    int c = 0;
    for (int i = 0; i < text->length; i++)
        len += text->paragraphs[i]->length;

    int lastlen = text->paragraphs[0]->length, curlen = lastlen;
    text->paragraphs[0]->sentences = realloc(text->paragraphs[0]->sentences, len * sizeof(struct Sentence *));

    for (int i = 1; i < text->length; i++) {
        struct Paragraph *temppar = text->paragraphs[i];
        for (int j = 0; j < temppar->length; j++) {
            text->paragraphs[0]->sentences[curlen++] = temppar->sentences[j];
        }
        free(temppar->sentences);
        free(temppar);
    }
    qsort(text->paragraphs[0]->sentences, len, sizeof(struct Sentence *), sntcmptask);
    text->paragraphs = realloc(text->paragraphs, sizeof(struct Paragraph *));
    text->paragraphs[0]->length = len;
    text->length = 1;
}
