#ifndef ENTITYVALUE_H_INCLUDED
#define ENTITYVALUE_H_INCLUDED

#include "chat1002.h"

enum intentType { WHO = 0, WHERE = 1, WHICH = 2 };

struct entityValue {
    // Key
    char entity[MAX_ENTITY];

    // Values
    char description[MAX_RESPONSE];
    enum intentType intent;
};

#endif // ENTITYVALUE_H_INCLUDED
