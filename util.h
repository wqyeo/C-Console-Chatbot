#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include "entityvalue.h"

int try_determine_intent(char* input);
bool try_get_entityValue(char* input, int input_size, enum intentType intent_type, struct entityValue* result);

//[[deprecated]]
char* try_get_entity(char* input, int size);

//[[deprecated]]
char* try_get_description(char* input, int size);

#endif // UTIL_H_INCLUDED
