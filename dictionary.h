#ifndef COURSEWORK_DICTIONARY_H
#define COURSEWORK_DICTIONARY_H

#include <stddef.h>

struct Hashtable {
    struct Item **items;
    int size;
};

struct Item {
    struct Item *next;
    wchar_t *key;
    void *value;
};

struct Hashtable *create_hashtable(int size);

void free_hashtable(struct Hashtable *table);

struct Item *find(struct Hashtable *hashtable, wchar_t *key);

struct Item *add(struct Hashtable *hashtable, wchar_t *key, void *value, size_t size);

#endif //COURSEWORK_DICTIONARY_H
