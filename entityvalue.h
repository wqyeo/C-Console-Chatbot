#ifndef ENTITYVALUE_H_INCLUDED
#define ENTITYVALUE_H_INCLUDED

#include "chat1002.h"

// The maximum number of entity-value pairing we can store in cache.
#define MAX_ENTITY_CACHE 512

enum intentType { WHO = 0, WHERE = 1, WHICH = 2 };

struct entityValue {
    // Key
    char entity[MAX_ENTITY];

    // Values
    char description[MAX_RESPONSE];
    enum intentType intent;
};

// Cache for loaded entity names paired to value.
struct entityValue entityCache[MAX_ENTITY_CACHE];

#endif // ENTITYVALUE_H_INCLUDED
