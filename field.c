/* based on genericDict.c in Professor Aspnes's notes */

#include <stdlib.h>
#include <string.h>
#include "field.h"
#include <stdio.h>

struct fieldElt {
    unsigned long hash;           /* full hash of key */
    void *key;
    void *value;
    struct fieldElt *next;
};

struct field {
    int tableSize;          /* number of slots in table */
    int numElements;        /* number of elements */
    size_t numShips;
    struct fieldElt **table; /* linked list heads */
    /* these save arguments passed at creation */
    struct keyOperations keyOps;
    struct valueOperations valueOps;
};

#define INITIAL_TABLESIZE (16)
#define TABLESIZE_MULTIPLIER (2)
#define TABLE_GROW_DENSITY (1)

Field 
fCreate(struct keyOperations keyOps,
            struct valueOperations valueOps)
{
    Field f;
    int i;

    f = malloc(sizeof(*f));
    if(f == 0) return 0;

    f->tableSize = INITIAL_TABLESIZE;
    f->numElements = 0;
    f->numShips = 0;
    f->keyOps = keyOps;
    f->valueOps = valueOps;
    f->table = malloc(sizeof(*(f->table)) * f->tableSize);
    if(f->table == 0) {
        free(f);
        return 0;
    }

    for(i = 0; i < f->tableSize; i++) f->table[i] = 0;

    return f;
}

void
fDestroy(Field f)
{
    int i;
    struct fieldElt *e;
    struct fieldElt *next;

    for(i = 0; i < f->tableSize; i++) {
        for(e = f->table[i]; e != 0; e = next) {
            next = e->next;
            f->keyOps.delete(e->key);
            f->valueOps.delete(e->value);
            free(e);
        }
    }
    free(f->table);
    free(f);
}

/* return pointer to element with given key, if any */
static struct fieldElt *
fFetch(Field f, const void *key)
{
    unsigned long h;
    int i;
    struct fieldElt *e;

    h = f->keyOps.hash(key);
    i = h % f->tableSize;
    for(e = f->table[i]; e != 0; e = e->next) {
        if(e->hash == h && f->keyOps.equal(key, e->key)) {
            /* found it */
            return e;
        }
    }
    /* didn't find it */
    return 0;
}

/* increase the size of the dictionary, rehashing all table elements */
static void
fGrow(Field f)
{
    struct fieldElt **old_table;
    int old_size;
    int i;
    struct fieldElt *e;
    struct fieldElt *next;
    int new_pos;

    /* save old table */
    old_table = f->table;
    old_size = f->tableSize;

    /* make new table */
    f->tableSize *= TABLESIZE_MULTIPLIER;
    f->table = malloc(sizeof(*(f->table)) * f->tableSize);
    if(f->table == 0) {
        /* put the old one back */
        f->table = old_table;
        f->tableSize = old_size;
        return;
    }
    /* else */
    /* clear new table */
    for(i = 0; i < f->tableSize; i++) f->table[i] = 0;

    /* move all elements of old table to new table */
    for(i = 0; i < old_size; i++) {
        for(e = old_table[i]; e != 0; e = next) {
            next = e->next;
            /* find the position in the new table */
            new_pos = e->hash % f->tableSize;
            e->next = f->table[new_pos];
            f->table[new_pos] = e;
        }
    }

    /* don't need this any more */
    free(old_table);
}

void
fSet(Field f, const void *key, const void *value)
{
	//printf("in fSet\n");
    int tablePosition;
    struct fieldElt *e;

    e = fFetch(f, key);
    if(e != 0) {
        /* change existing setting */
        f->valueOps.delete(e->value);
        e->value = value ? f->valueOps.copy(value) : 0;
    } else {
        /* create new element */
        e = malloc(sizeof(*e));
        if(e == 0) abort();

        e->hash = f->keyOps.hash(key);
        e->key = f->keyOps.copy(key);
        e->value = value ? f->valueOps.copy(value) : 0;

        /* link it in */
        tablePosition = e->hash % f->tableSize;
        e->next = f->table[tablePosition];
        f->table[tablePosition] = e;

        f->numElements++;

        if(f->numElements > f->tableSize * TABLE_GROW_DENSITY) {
            /* grow and rehash */
            fGrow(f);
        }
    }
}

const void *
fGet(Field f, const void *key)
{
    struct fieldElt *e;
    //printf("in fget!\n");

    e = fFetch(f, key);
    //printf("fetched key\n");
   // printf("still here\n");
    
    if(e != 0) {
    //	printf("key exists\n");
        return e->value;
    } else {
    //	printf("key doesn't exist");
        return 0;
    }
}

size_t
fNumShips(const struct field *f) {
	return f->numShips;
}

void
fAddShip(Field f) {
	f->numShips++;
}

void
fDeleteShip(Field f) {
	if (f->numShips > 0) {f->numShips--;}
}



