#ifndef _SHELL_H_
#define _SHELL_H_

/* * Required Headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

/* Global Variables */
extern char **environ;
extern int last_exit_status; /* To store the last command's exit status */

/* * --- Function Prototypes --- */

/* A) Main Loop & Prompt */
int handle_command(char *line, char *prog_name);
void print_prompt(void);

/* B) Parsing & Tokenization */
char **tokenize_line(char *line);
int check_for_whitespace(char *str);

/* C) Execution */
int execute_command(char **args, char *prog_name);

/* D) PATH Handling */
char *find_path(char *command);
char *build_path(char *dir, char *command);

/* E) Built-in Commands (Tasks 5 & 6) */
int check_builtins(char **args);
void print_env(void);
void handle_exit(char **args);

/* F) Utilities & Error Handling */
void print_error(char *prog_name, char *command);

#endif /* _SHELL_H_ */
