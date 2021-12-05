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

//struct Hashtable *resize(struct Hashtable *hashtable) {
//    int prevsize = hashtable->size;
//    int size = prevsize * 2;
//    struct Hashtable *nhashtable = create_hashtable(size);
//}

struct Item *find(struct Hashtable *hashtable, wchar_t *key) {
    struct Item *n;
    for (n = hashtable->items[hash(key, hashtable->size)]; n; n = n->next) {
        if (n->key == NULL || key == NULL)
            return NULL;
        if (wcscmp(key, n->key) == 0)
            return n;
    }
    return NULL;
}

struct Item *add(struct Hashtable *hashtable, wchar_t *key, void *value, size_t size) {
    struct Item *n;
    unsigned hashval;
    void *data = malloc(size);
    if (!data)
        return NULL;
    if (!(n = find(hashtable, key))) {
        if (!(n = malloc(sizeof(*n))) || !(n->key = wcsdup(key))) {
            free(data);
            free(n);
            return NULL;
        }
        hashval = hash(key, hashtable->size);
        n->next = hashtable->items[hashval];
        hashtable->items[hashval] = n;
    } else
        free(n->value);
    memcpy(data, value, size);
    n->value = data;
    return n;
}
