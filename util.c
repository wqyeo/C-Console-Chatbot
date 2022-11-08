#include "util.h"
#include "entityvalue.h"

#pragma region Knowledge Utils
// Determine intent from string
// NULL if invalid.
int try_determine_intent(char* input){
    if (strcmp(input, "[what]") == 0){
        return WHAT;
    } else if (strcmp(input, "[where]") == 0) {
        return WHERE;
    } else if (strcmp(input, "[who]") == 0){
        return WHO;
    }

    return NULL;
}

bool try_get_entityValue(char* input, int input_size, enum intentType intent_type, struct entityValue* result){
    result->intent = intent_type;

    bool found_equal_sign = false;

    int entity_size_count = 0;

    int i;
    for (i = 0; i < input_size; ++i){
        if (entity_size_count > MAX_ENTITY){
            // Out of space to store entity.
            return false;
        }

        if (i == '='){
            found_equal_sign = true;
            result->entity[i] = '\0';
            continue;
        }

        if (found_equal_sign){
            // Desc is everything after the '=' sign.
            result->description[i] = input[i];
        } else {
            // entity is everything before.
            result->entity[i] = input[i];
            ++entity_size_count;
        }
    }

    return true;
}

// Determine the entity name from input.
// Null if invalid.
char* try_get_entity(char* input, int size){
    char result[MAX_ENTITY];

    int i;
    for (i = 0; i < size; ++i){
        if (i == MAX_ENTITY){
            // Out of space to store entity.
            return NULL;
        }
        if (i == '='){
            result[i] = '\0';
        } else {
            result[i] = input[i];
        }
    }
    return result;
}

char* try_get_description(char* input, int size){
    char result[MAX_INPUT];

    bool found_equal_sign = false;
    int i;
    for (i = 0; i < size; ++i){
        if (i == MAX_INPUT){
            // Out of space to store entity.
            return NULL;
        }

        if (found_equal_sign){
            // Desc is everything after the '=' sign.
            result[i] = input[i];
        }
        if (i == '='){
            found_equal_sign = true;
        }
    }
    return result;
}
#pragma endregion

#pragma region Entity Cache Utils

bool try_get_entityValue_by(enum intentType intent_type, char* entity_name, struct entityValue* result) {
    int i;
    for (i = 0; i < MAX_ENTITY_CACHE; ++i){
        if (entity_cache[i].intent == NULL || entity_cache[i].intent == EMPTY){
            // Hit the end; Probably doesn't exists.
            return false;
        }

        if (strcmp(entity_cache[i].entity, entity_name) == 0 && intent_type == entity_cache[i].intent){
            // Found that fits condition.
            result = &(entity_cache[i]);
            return true;
        }
    }

    return false;
}

// Insert at end. Replace description if conflicting entity+intent is found.
bool try_insertReplace_cache(struct entityValue element){
    int i;
    for (i = 0; i < MAX_ENTITY_CACHE; ++i){
        if (entity_cache[i].intent == NULL || entity_cache[i].intent == EMPTY){
            // Add at the end.
            entity_cache[i] = element;
            ++i;
            if (i < MAX_ENTITY_CACHE){
                // Expand the end, if there is still space.
                entity_cache[i].intent = EMPTY;
            }
            return true;
        }

        if (strcmp(entity_cache[i].entity, element.entity) == 0 && element.intent == entity_cache[i].intent) {
            // Found a conflicting entity/element, replace instead.
            entity_cache[i] = element;
            return true;
        }
    }

    return false;
}

// Size of entity cache
int get_current_cache_size(){
    int i;
    for (i = 0; i < MAX_ENTITY_CACHE; ++i){
        if (entity_cache[i].intent == NULL || entity_cache[i].intent == EMPTY){
            break;
        }
    }
    return i;
}
#pragma endregion
