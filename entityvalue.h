#ifndef ENTITYVALUE_H_INCLUDED
#define ENTITYVALUE_H_INCLUDED

#include "chat1002.h"

// The maximum number of entity-value pairing we can store in cache.
#define MAX_ENTITY_CACHE 512

// U
enum intentType { EMPTY = 0, WHO = 1, WHERE = 2, WHAT = 3 };

struct entityValue {
    // Key
    char entity[MAX_ENTITY];

    // Values
    char description[MAX_RESPONSE];
    enum intentType intent;
};

// Cache for loaded entity names paired to value.
// Check util.h for functions to assists in using this cache.
struct entityValue entity_cache[MAX_ENTITY_CACHE];

#endif // ENTITYVALUE_H_INCLUDED
