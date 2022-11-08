#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include "entityvalue.h"

int try_determine_intent(char* input);
bool try_get_entityValue(char* input, int input_size, enum intentType intent_type, struct entityValue* result);

//[[deprecated]]
char* try_get_entity(char* input, int size);
//[[deprecated]]
char* try_get_description(char* input, int size);

// Utils with entity_cache.
bool try_insertReplace_cache(struct entityValue element);
bool try_get_entityValue_by(enum intentType intent_type, char* entity_name, struct entityValue* result);
int get_current_cache_size();

#endif // UTIL_H_INCLUDED
