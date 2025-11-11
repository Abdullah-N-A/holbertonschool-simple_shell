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

/* Global Environment Variable */
extern char **environ;

/* * --- Function Prototypes --- */

/* A) Main Loop & Prompt */
int run_shell_loop(char **av);
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
/* Assuming you will use standard string.h functions for simplicity
 * But if restricted, you'd implement _strlen, _strcmp, etc., here.
 */

#endif /* _SHELL_H_ */
