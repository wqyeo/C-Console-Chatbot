#include "util.h"
#include "entityvalue.h"

#pragma region Knowledge Utils
// Determine intent from string
// EMPTY if invalid.
int try_determine_intent(char* input){
    if (strcmp(input, "[what]") == 0 || strcmp(input, "what") == 0){
        return WHAT;
    } else if (strcmp(input, "[where]") == 0|| strcmp(input, "where") == 0) {
        return WHERE;
    } else if (strcmp(input, "[who]") == 0|| strcmp(input, "who") == 0){
        return WHO;
    }

    return EMPTY;
}

bool try_get_entityValue(char* input, int input_size, enum intentType intent_type, struct entityValue* result){
    result->intent = intent_type;

    bool found_equal_sign = false;
    int entity_size_count = 0;

    int i;
    int description_index_diff = 0;
    for (i = 0; i < input_size && input[i] != '\0'; ++i){
        if (entity_size_count > MAX_ENTITY){
            // Out of space to store entity.
            return false;
        }

        if (input[i] == '=' && !found_equal_sign){
            // Found first equal sign, stop adding to entity.
            // Now we start adding to description.
            found_equal_sign = true;
            description_index_diff = i + 1;
            if (i < MAX_ENTITY) {
                // end if there is still space.
                result->entity[i] = '\0';
            }
            continue;
        }

        if (found_equal_sign){
            // Desc is everything after the '=' sign.
            result->description[i - description_index_diff] = input[i];
        } else {
            // entity is everything before.
            result->entity[i] = input[i];
            ++entity_size_count;
        }
    }

    if (!found_equal_sign){
        // Description was not filled in.
        return false;
    }
    if (i < MAX_RESPONSE) {
        // End description if there is still space.
        result->description[i - description_index_diff] = '\0';
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


bool is_linking_verb(const char *input){
    int linking_verbs_size = 3;
    char* linking_verb[] = {
        "are",
        "is",
        "was"
    };

    int i;
    for (i = 0; i < linking_verbs_size; ++i){
        if (compare_token(linking_verb[i], input) == 0){
            // The input is one of the linking verbs.
            return true;
        }
    }
    // Doesnt exist in any of the linking verbs.
    return false;
}

#pragma endregion

#pragma region Entity Cache Utils

// Mostly for debugging
void print_cache(){
    int i;
    for (i = 0; i < MAX_ENTITY_CACHE; ++i){
        if (entity_cache[i].intent == EMPTY || entity_cache[i].intent == NULL){
            // Hit the end;
            return;
        }

        printf("%d :: ", entity_cache[i].intent);
        printf("%s=", entity_cache[i].entity);
        printf("%s\n\0", entity_cache[i].description);
    }
    return;
}

bool try_get_entityValue_by(enum intentType intent_type, char* entity_name, struct entityValue *result) {
    int i;
    for (i = 0; i < MAX_ENTITY_CACHE; ++i){
        if (entity_cache[i].intent == EMPTY || entity_cache[i].intent == NULL){
            // Hit the end; Probably doesn't exists.
            return false;
        }
#if defined(LOG_UTIL) && LOG_UTIL
        printf("\FETCH COMPARE (%s against %s)\n", entity_cache[i].entity, entity_name);
#endif

        if (strcmp(entity_cache[i].entity, entity_name) == 0 && intent_type == entity_cache[i].intent){
            // Found that fits condition.
            strncpy(result->description, entity_cache[i].description, MAX_RESPONSE);
            strncpy(result->entity, entity_cache[i].entity, MAX_ENTITY);
            result->intent = entity_cache[i].intent;
            return true;
        }
    }

    return false;
}

// Insert at end. Replace description if conflicting entity+intent is found.
bool try_insertReplace_cache(struct entityValue element){
    // Loop through, find either a conflicting to replace
    // or an empty slot to insert into.
    int i;
    for (i = 0; i < MAX_ENTITY_CACHE; ++i){
        if (entity_cache[i].intent == EMPTY || entity_cache[i].intent == NULL){
            // Empty slot found, insert.
            entity_cache[i].intent = element.intent;
            strncpy(entity_cache[i].description, element.description, MAX_RESPONSE);
            strncpy(entity_cache[i].entity, element.entity, MAX_ENTITY);

            ++i;
            if (i < MAX_ENTITY_CACHE){
                // Expand the end, if there is still space.
                entity_cache[i].intent = EMPTY;
            }
#if defined(LOG_UTIL) && LOG_UTIL
            printf("INSERT Cache (%d :: ", element.intent);
            printf("%s = ", element.entity);
            printf("%s)\n", element.description);
#endif
            return true;
        }

        if (strcmp(entity_cache[i].entity, element.entity) == 0 && element.intent == entity_cache[i].intent) {
            // Found a conflicting entity/element, replace instead.
#if defined(LOG_UTIL) && LOG_UTIL
            printf("REPLACE Cache (%d :: ", element.intent);
            printf("%s = ", element.entity);
            printf("%s)\n", element.description);
#endif
            strncpy(entity_cache[i].description, element.description, MAX_RESPONSE);
            return true;
        }
    }

    // Ran out of space.
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
