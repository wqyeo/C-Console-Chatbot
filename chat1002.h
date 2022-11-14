/*
 * INF1002 (C Language) Group Project.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the INF1002 chatbot.
 */

#ifndef _CHAT1002_H
#define _CHAT1002_H

#include <stdio.h>
#include <stdbool.h>

/* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_INPUT    256

/* the maximum number of characters allowed in the name of an intent (including the terminating null)  */
#define MAX_INTENT   32

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_RESPONSE 256

// Maximum number of characters allowed for a file name.
#define MAX_FILE_NAME 32

// Maximum number of characters allowed for a name
#define MAX_NAME 32

char chatbot_name[MAX_NAME];
char user_name[MAX_NAME];

/* return codes for knowledge_get() and knowledge_put() */
enum KB_Code { KB_NOMEM = -3, KB_INVALID = -2, KB_NOTFOUND = -1, KB_OK = 0 };

/* functions defined in main.c */
int compare_token(const char *token1, const char *token2);
void prompt_user(char *buf, int n, const char *format, ...);
char* concatenate(int args_count, ...);
bool is_whitespace_or_empty(const char *input, int size);

/* functions defined in chatbot.c */
const char *chatbot_botname();
const char *chatbot_username();
int chatbot_main(int inc, char *inv[], char *response, int n);
bool chatbot_is_exit(const char *intent);
int chatbot_do_exit(int inc, char *inv[], char *response, int n);
bool chatbot_is_load(const char *intent);
int chatbot_do_load(int inc, char *inv[], char *response, int n);
bool chatbot_is_question(const char *intent);
int chatbot_do_question(int inc, char *inv[], char *response, int n);
bool chatbot_is_reset(const char *intent);
int chatbot_do_reset(int inc, char *inv[], char *response, int n);
bool chatbot_is_save(const char *intent);
int chatbot_do_save(int inc, char *inv[], char *response, int n);
// command for setting name
bool chatbot_is_set_name(int inc, char* inv[]);
int chatbot_set_name(int inc, char* inv[], char* response, int n);

/* functions defined in knowledge.c */
enum KB_Code knowledge_get(int intent, const char *entity, char *response, int n);
enum KB_Code knowledge_put(int intent, const char *entity, const char *response);
void knowledge_reset();
int knowledge_read(FILE *f);
void knowledge_write(FILE *f);

#endif
