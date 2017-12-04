#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <pwd.h>
#include <fcntl.h>

/*======== void print_shell_prompt() ==========
  Inputs: NONE
  Returns: NONE

  Prints out the shell prompt of the user, as would normally
  be seen in bash.
  =============================================*/
void print_shell_prompt();

/*======== char * read_line() ==========
  Inputs: NONE
  Returns: Line read from stdin
  Otherwise, exits.
  Uses C function getline(char **lineptr, size_t *n, FILE *stream) [man 3]
  Tries to read line from stdin,
  If failed, exits program.
  =======================================*/
char * read_line();


/*======== char ** parse_args() ==========
  Inputs: char * line, char * delim
  Returns: char ** [array of strings previously separated by delim]

  Starts with parsing five arguments using strsep
  If more than five args, array dynamically resized with +5 more arguments
  =========================================*/
char ** parse_args( char * line, char * delim );

/*======== void strip_newline() ==========
  Inputs: char * str
  Returns: NONE

  Removes the newline character from the end of a string.
  =========================================*/
void strip_newline( char *str );

/*======== char* trim() ==========
  Inputs: char * c
  Returns: Pointer to beginning of string (w/o spaces)

  Removes the spaces in front of and behind a string.
  *c removes spaces from the front.
  *e removes from back, and ensures null termination.
  isspace() function used to check if pointing to space.
  ================================*/
char* trim();

/*======== int check_special(char * cmd) ========
  Inputs: char * cmd
  Returns: int id# representing what type of special command

  Checks if the cmdline input involves redirection or piping.
  Returns an integer identifier based on special character found:
  > : 1; < : 2; | : 3
  Returns 0 if no pipes or redirection characters found.
  Returns -1 if >>, >&, >>&, 1>, 1>>, 2>, 2>>, &>, or &>> used (not supported)
  ===============================================*/

int check_special(char * cmd);

/*======== int size(char ** args) ==========
  Inputs: char ** args
  Returns: int size

  Finds the size of an array of strings.
  ==========================================*/
int size(char ** args);

/*======== void pipredir(int id, char * cmd, char * exec) ==========
  Inputs: int id, char * cmd, char * exec
  Returns: NONE

  Executes the redirection and piping functionality of the shell.
  id == 1: signals < redirection
  Parses arguments with < delimiter
  Chaining - if more than one '<', only the last one is read as stdin like bash. If there are '>' afterwards, only the file after the last one will get written to.
  Fails if file doesn't exist like bash.
  id == 2: signals > redirection
  Parses arguments with > delimiter
  Chaining - if more than one '>', only the last one gets written in like bash.
  id == 3: signals | redirection
  Parses arguments with | delimiter
  Consecutive piping not implemented.
  =====================================================*/
void pipredir(int id, char * cmd, char * exec);

/*======== void fork_exec(char ** args) ==========
  Inputs: char * str
  Returns: NONE

  Takes an array of arguments and executes them using execvp OR
  if exit command entered: exit the shell
  if cd command entered: use chdir() to move into another directory
  All other commands: fork off process and execvp.
  [Piping and Redirection accounted for in pipredir() fxn above ^^]
  ================================================*/
void fork_exec( char ** args );

/*======== void exec_all(char * input) ==========
  Inputs: char * input
  Returns: NONE

  Takes in input from the user and calls functions defined above
  based on the situation: e.g., if '<' character exists, then
  call the function that deals with output redirection.
  ===============================================*/
void exec_all( char * input );
