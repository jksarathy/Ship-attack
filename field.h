#include <stdlib.h>
#include <string.h>

typedef struct field *Field;

struct keyOperations {
    unsigned long (*hash)(const void *datum);

    int (*equal)(const void *datum1, const void *datum2);

    void *(*copy)(const void *datum);

    void (*delete)(void *datum);
};

struct valueOperations {
	void *(*copy)(const void *datum);
	
	void (*delete)(void *datum);
};

/* create a new dictionary with given key and value operations */
/* Note: valueOps.hash and valueOps.equal are not used. */
Field fCreate(struct keyOperations keyOps,
                 struct valueOperations valueOps);

/* free a dictionary and all the space it contains */
/* This will call the appropriate delete function for all keys and */
/* values. */
void fDestroy(Field f);

/* Set dict[key] = value. */
/* Both key and value are copied internally. */
/* If data is the null pointer, remove dict[key]. */
void fSet(Field f, const void *key, const void *value);

/* Return dict[key], or null if dict[key] has not been set. */
const void *fGet(Field f, const void *key);

size_t fNumShips(const struct field *f);

void fAddShip(Field f);

void fDeleteShip(Field f);
