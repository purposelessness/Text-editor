#include <wchar.h>
#include <malloc.h>
#include <string.h>
#include "dictionary.h"

unsigned hash(wchar_t *key, int size) {
    unsigned hashval;
    for (hashval = 0; *key != '\0'; key++)
        hashval = *key + 31 * hashval;
    return hashval % size;
}

struct Hashtable *create_hashtable(int size) {
    struct Hashtable *table = malloc(sizeof(struct Hashtable));
    table->size = size;
    table->items = calloc(size, sizeof(struct Item));
    return table;
}

void free_hashtable(struct Hashtable *table) {
    struct Item *item;
    for (int i = 0; i < table->size; i++) {
        item = table->items[i];
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
    struct Item *item;
    for (int i = 0; i < prevsize; i++) {
        item = hashtable->items[i];
        if (item)
            add(&nhashtable, item->key, item->value, sizeof(item->value));
    }
    free_hashtable(hashtable);
    return nhashtable;
}

struct Item *find(struct Hashtable *hashtable, wchar_t *key) {
    return hashtable->items[hash(key, hashtable->size)];
}

struct Item *add(struct Hashtable **hashtable, wchar_t *key, void *value, size_t size) {
    struct Item *n;
    unsigned hashval;
    void *data = malloc(size);
    if (!data)
        return NULL;
    if (!(n = find(*hashtable, key))) {
        if (!(n = malloc(sizeof(*n))) || !(n->key = wcsdup(key))) {
            free(data);
            return NULL;
        }
        hashval = hash(key, (*hashtable)->size);
        (*hashtable)->items[hashval] = n;
    } else {
        if (wcscmp(n->key, key) != 0) {
            *hashtable = resize(*hashtable);
            free(data);
            return add(hashtable, key, value, size);
        }
        free(n->value);
    }
    memcpy(data, value, size);
    n->value = data;
    return n;
}
