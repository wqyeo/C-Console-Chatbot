#include "util.h"
#include "entityvalue.h"

// Determine intent from string
// NULL if invalid.
int try_determine_intent(char* input){
    if (input == "[what]"){
        return WHAT;
    } else if (input == "[where]") {
        return WHERE;
    } else if (input == "[who]"){
        return WHO;
    }

    return NULL;
}
bool try_get_entityValue(char* input, int input_size, enum intentType intent_type, struct entityValue* result){
    result -> intent = intent_type;

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
