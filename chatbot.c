/*
 * INF1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "entityvalue.h"
#include "chat1002.h"


/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {
	return "User";
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {

	/* check for empty input */
	if (inc <= 0) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}
}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
bool chatbot_is_exit(const char *intent) {

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
bool chatbot_is_load(const char *intent) {
	return (compare_token(intent, "load") == 0);
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {
    if (inc < 2) {
        // No params given after command.
        strncpy(response, "Indicate the file name to load from. Ex: 'LOAD from INF1002'", n);
        return 0;
    }

    // Check if the 2nd word is "from".
    int linking_verb_flag = 0;
    if (compare_token(inv[1], "from") == 0){
        linking_verb_flag = 1;
    }

    if (linking_verb_flag == 1 && inc < 3) {
        // No file given to load from.
        strncpy(response, "Indicate the file name to load from. Ex: 'LOAD from INF1002'", n);
        return 0;
    }

    // TODO: Check if the user gave a '.ini' at the end.
    // Append if not, else do nothing.
    char* file_name = concatenate(2, inv[linking_verb_flag + 1], ".ini");
    strncpy(response, concatenate(3, "Loading configuration from '", file_name, "'.\n"), n);

#if defined(LOG_CHATBOT) && LOG_CHATBOT
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working directory: %s\n", cwd);
    } else {
       perror("getcwd() error");
    }
#endif

    FILE* ini_file;
    ini_file = fopen(file_name, "r");

    if (ini_file == NULL) {
        // Failed to open file; Probably doesn't exists.
        strncpy(response, concatenate(2, response, "\nFailed to load file. Does the configuration exists?\n(HINT: Do not add '.ini')"), n);
        return 0;
    }

    // Read and load
    int readed_count = knowledge_read(ini_file);
    if (readed_count > 0){
        char buffer[MAX_RESPONSE];
        sprintf(buffer, "Loaded %d knowledge from configuration.", readed_count);
        strncpy(response, concatenate(2, response, buffer), n);
    } else {
        strncpy(response, concatenate(2, response, "The given file is invalid. No knowledge was loaded."), n);
    }

    fclose(ini_file);
	return 0;
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
bool chatbot_is_question(const char *intent) {
	return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0;
}


/*
 * Answer a question.
 * * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

    // No params given after command.
    if (inc < 2) {
        // TODO: Print warning message or something.
        return 0;
    }

    // Check "is" or "are".
    int linking_verb_flag = 0;
    if (is_linking_verb(inv[1])){
        // TODO: Should we force linking verbs?
        linking_verb_flag = 1;
    }

    // No entity given.
    if (linking_verb_flag == 1 && inc < 3) {
        // TODO: Print warning message or something.
        return 0;
    }

    // Get entity
    char input_entity[MAX_ENTITY];
    if (try_combine(inv, ' ', 1 + linking_verb_flag, inc, MAX_ENTITY, input_entity)) {
        knowledge_get(try_determine_intent(inv[0]), input_entity, response, n);
    } else {
        strncpy(response, "I ran out of space to process the entity name...", n);
    }

	return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
bool chatbot_is_reset(const char *intent) {
	return compare_token(intent, "reset") == 0;
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
	knowledge_reset();
	strncpy(response, "I forgotten everything.", n);
	return 0;
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
bool chatbot_is_save(const char *intent) {

    return compare_token(intent, "save") == 0;
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {

    if (inc < 2){
        // No params after command.
        strncpy(response, "I need a file name to save under.\nExample: 'Save to INF1002'.\nHINT: Do not include '.ini'", n);
        return 0;
    }

    int linking_verb_flag = 0;
    if (compare_token(inv[1], "to") == 0){
        linking_verb_flag = 1;
    }

    if (linking_verb_flag == 1 && inc < 3){
        // No file name given.
        strncpy(response, "I need a file name to save under.\nExample: 'Save to INF1002'.\nHINT: Do not include '.ini'", n);
        return 0;
    }

    // +5, space for file extension
    char file_name[MAX_FILE_NAME + 5];
    if (try_combine(inv, '_', 1 + linking_verb_flag, inc, MAX_FILE_NAME, file_name)){
        // Append file extension
        strncpy(file_name, concatenate(2, file_name, ".ini"), MAX_FILE_NAME + 5);

        FILE *fp;
        if((fp = fopen(file_name,"r"))!=NULL) {
            // If file exists already, warn overwrite.
            fclose(fp);
            printf("WARN: the file '%s' exists before, it will be overwritten...\n", file_name);
        }

        // Open and write
        fp = fopen(file_name, "w+");
        knowledge_write(fp);
        fclose(fp);

        strncpy(response, concatenate(3, "Saved my current knowledge to '", file_name, "'."), n);
    } else {
        strncpy(response, "File name is too long!\nHINT: Do not include '.ini'", n);
    }

	return 0;

}



