/*
 * INF1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
enum KB_Code knowledge_get(enum KB_Code intent, const char *entity, char *response, int n) {

    if (intent == EMPTY){
        // Unknown intent
        strncpy(response, "Unknown intent.\nHint: Where, Who, What", n);
        return KB_INVALID;
    }

    struct entityValue found_response;
	if (!try_get_entityValue_by(intent, entity, &found_response)){
        // Not found.
        printf("%s: I do not know how to respond to that. Please let me know how to respond:\n", chatbot_botname());

        // Get response
        char response_input[MAX_RESPONSE];
        fgets(response_input, MAX_RESPONSE, stdin);
        // This removes the newline from the response (when user hits enter).
        response_input[strlen(response_input) - 1] = '\0';

        // Store Response
        enum KB_Code return_code;
        return_code = knowledge_put(intent, entity, response_input);

        if (return_code == KB_NOTFOUND){
            strncpy(response, "oof", n);
        } else if (return_code == KB_NOMEM){
            strncpy(response, "I ran out of memory to store more information!", n);
        } else if (return_code == KB_OK) {
            strncpy(response, concatenate(5, "Got it, I will respond to '", entity, "' with '", response_input, "'."), n);
        }

        return return_code;
	}
	// Else, if we found a response, just echo
    strncpy(response, found_response.description, n);
	return KB_OK;
}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
enum KB_Code knowledge_put(enum KB_Code intent, const char *entity, const char *response) {

    if (is_whitespace_or_empty(response, strlen(response))){
        return KB_NOTFOUND;
    }

    // Create object and fill.
    struct entityValue to_store;
    to_store.intent = intent;
    strncpy(to_store.entity, entity, MAX_ENTITY);
    strncpy(to_store.description, response, MAX_RESPONSE);

    // Insert into cache.
    if (!try_insertReplace_cache(to_store)){
        // Fail to insert. Probably out of memory.
#if defined(LOG_KNOWLEDGE) && LOG_KNOWLEDGE
        printf("FAILED INSERT GET (%s)\n", response);
#endif
        return KB_NOMEM;
    }

	return KB_OK;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {
    int readed_pairs = 0;

    enum intentType curr_intent_type = WHAT;
    char current_line[MAX_INPUT];

	// Continuously read until end of file
	// (or until error)
	while (fgets(current_line, MAX_INPUT, f) != NULL){
        if (is_whitespace_or_empty(current_line, MAX_INPUT)){
            // Whitespaces, skip this.
            continue;
        }
        // Remove any newline from the current line.
        current_line[strcspn(current_line, "\n")] = 0;

#if defined(LOG_KNOWLEDGE) && LOG_KNOWLEDGE
    printf("\nCURRENT LINE (%s)\n", current_line);
#endif

        int intent = try_determine_intent(current_line);

        if (intent != EMPTY){
            // Change intent type, and go next line.
            curr_intent_type = intent;

#if defined(LOG_KNOWLEDGE) && LOG_KNOWLEDGE
    printf("INTENT CHANGE (%d)\n", curr_intent_type);
#endif
            continue;
        }

        struct entityValue curr_entityValue;
        if (!try_get_entityValue(current_line, MAX_INPUT, curr_intent_type, &curr_entityValue) || !try_insertReplace_cache(curr_entityValue)){
            // Failed to get entityvalue, or failed to insert into cache.
            // TODO: error message or smth.
            --readed_pairs;
        }
        ++readed_pairs;
	}

#if defined(LOG_KNOWLEDGE) && LOG_KNOWLEDGE
    printf("\nLOADED CACHE, PRINTING...\n");
	print_cache();
#endif

#if defined(TEST_CASE) && TEST_CASE
    // Test Cache search (Based on INF1002.ini)
    struct entityValue test_case;
    if (try_get_entityValue_by(WHO, "Frank Guan", &test_case)) {
        printf("\n\nTEST CASE (%d :: ", test_case.intent);
        printf("%s = ", test_case.entity);
        printf("%s)\n", test_case.description);

        // Test replacing...
        strncpy(test_case.description, "Blin", 256);
        if (try_insertReplace_cache(test_case)){
            printf("REPLACE SUCCESS CODE\n\n");
            if (try_get_entityValue_by(WHO, "Frank Guan", &test_case)) {
                printf("CHECK REPLACE (%d :: ", test_case.intent);
                printf("%s = ", test_case.entity);
                printf("%s)\n", test_case.description);
            }
        }
    }
#endif

	return readed_pairs;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

	/* to be implemented */

}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {

	/* to be implemented */

}
