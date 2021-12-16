#ifndef COURSEWORK_HASHTABLE_H
#define COURSEWORK_HASHTABLE_H

#include <stddef.h>

struct Hashtable {
    struct Item **items;
    int size;
};

struct Item {
    wchar_t *key;
    void *value;
    size_t size;
};

struct Hashtable *create_hashtable(int size);

void free_hashtable(struct Hashtable *table);

struct Item *find(struct Hashtable *hashtable, wchar_t *key);

struct Item *add(struct Hashtable **hashtable, wchar_t *key, void *value, size_t size);

#endif //COURSEWORK_HASHTABLE_H
