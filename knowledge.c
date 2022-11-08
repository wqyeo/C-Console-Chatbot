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
int knowledge_get(const char *intent, const char *entity, char *response, int n) {

	/* to be implemented */

	return KB_NOTFOUND;
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
int knowledge_put(const char *intent, const char *entity, const char *response) {

	/* to be implemented */

	return KB_INVALID;

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
	fgets(current_line, MAX_INPUT, (FILE*)f);


	// Continuously read until end of file
	// (or until error)
	while (current_line != NULL){
        if (is_whitespace_or_empty(current_line, MAX_INPUT)){
            // Whitespaces, skip this.
            fgets(current_line, MAX_INPUT, (FILE*)f);
            continue;
        }

        int intent = try_determine_intent(current_line);
        if (intent != NULL){
            // Change intent type.
            curr_intent_type = intent;
        }

        struct entityValue curr_entityValue;
        if (!try_get_entityValue(current_line, MAX_INPUT, curr_intent_type, &curr_entityValue)){
            // Failed to get entityvalue
            // TODO: error message or smth.
            --readed_pairs;
        } else if (!try_insertReplace_cache(curr_entityValue)){
            // Failed to insert into cache
            // TODO: error message
            --readed_pairs;
        }

        ++readed_pairs;
        // Read next line.
        fgets(current_line, MAX_INPUT, (FILE*)f);
	}

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
