#include <wchar.h>
#include <malloc.h>
#include <string.h>
#include "hashtable.h"

unsigned hash(wchar_t *key, int size) {
    unsigned hashval;
    for (hashval = 0; *key != '\0'; key++)
        hashval = *key + 31 * hashval;
    return hashval % size;
}

struct Hashtable *create_hashtable(int size) {
    struct Hashtable *table = malloc(sizeof(struct Hashtable));
    if (!table) {
        wprintf(L"Memory allocation error\n");
        return NULL;
    }
    table->size = size;
    table->items = calloc(size, sizeof(struct Item));
    if (!table->items) {
        wprintf(L"Memory allocation error\n");
        free(table);
        return NULL;
    }
    return table;
}

void free_hashtable(struct Hashtable *table) {
    for (int i = 0; i < table->size; i++) {
        struct Item *item = table->items[i];
        if (item == NULL)
            continue;
        free(item->key);
        free(item->value);
        free(item);
    }
    free(table->items);
    free(table);
}

struct Hashtable *resize(struct Hashtable *hashtable) {
    int prevsize = hashtable->size;
    struct Hashtable *nhashtable = create_hashtable(prevsize * 2);
    if (!nhashtable) {
        free_hashtable(hashtable);
        return NULL;
    }
    struct Item *item;
    for (int i = 0; i < prevsize; i++) {
        item = hashtable->items[i];
        if (item)
            add(&nhashtable, item->key, item->value, item->size);
    }
    free_hashtable(hashtable);
    return nhashtable;
}

struct Item *find(struct Hashtable *hashtable, wchar_t *key) {
    struct Item *item = hashtable->items[hash(key, hashtable->size)];
    return item && (wcscmp(key, item->key) == 0) ? item->value : NULL;
}

struct Item *softfind(struct Hashtable *hashtable, wchar_t *key) {
    return hashtable->items[hash(key, hashtable->size)];
}

struct Item *add(struct Hashtable **hashtable, wchar_t *key, void *value, size_t size) {
    struct Item *item;
    unsigned hashval;
    void *data = malloc(size);
    if (!data) {
        wprintf(L"Memory allocation error\n");
        return NULL;
    }
    if (!(item = softfind(*hashtable, key))) {
        if (!(item = malloc(sizeof(*item))) || !(item->key = wcsdup(key))) {
            wprintf(L"Memory allocation error");
            free(data);
            return NULL;
        }
        hashval = hash(key, (*hashtable)->size);
        (*hashtable)->items[hashval] = item;
    } else {
        free(data);
        if (wcscmp(item->key, key) != 0) {
            *hashtable = resize(*hashtable);
            if (!hashtable) {
                wprintf(L"Cannot add element!\n");
                return NULL;
            }
            return add(hashtable, key, value, size);
        }
        wprintf(L"Element with same key already exists!\n");
        return NULL;
    }
    memcpy(data, value, size);
    item->value = data;
    item->size = size;
    return item;
}
