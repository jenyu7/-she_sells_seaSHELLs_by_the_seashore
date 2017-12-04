# she_sells_seaSHELLs_by_the_seashore  
*by Adeebur Rahman and Jen Yu*  

## Features:  
* Forks and executes commands.  
* Parses multiple commands on one line.  
* Redirects using >, <.  
- Chaining redirections like `ls > a > b > c`, `wc < a < b < c`, and `wc < a > b > c` work like they would in bash.  
* Simple piping, such as `ls | wc` works.  
* Prints out shell prompt with user, hostname and current working directory.  

## Attempted:  
**The following did not end up working, but have been left in the code, commented out.**  

## Bugs:  


## Files & Function Headers:  
`fxn.c`  
**Contains all functions used within project**  

/\*======== void print_shell_prompt() ==========  
Inputs: NONE  
Returns: NONE  
  
Prints out the shell prompt of the user, as would normally  
be seen in bash.  
=============================================\*/  
  
/\*======== char \* read_line() ==========  
Inputs: NONE  
Returns: Line read from stdin  
Otherwise, exits.  
Uses C function getline(char **lineptr, size_t *n, FILE *stream) [man 3]  
Tries to read line from stdin,  
If failed, exits program.  
=======================================\*/  
  
/\*======== char \*\* parse_args() ==========  
Inputs: char \* line, char \* delim  
Returns: char \*\* [array of strings previously separated by delim]  
  
Starts with parsing five arguments using strsep  
If more than five args, array dynamically resized with +5 more arguments  
=========================================\*/  
  
/\*======== void strip_newline() ==========  
Inputs: char \* str  
Returns: NONE  
  
Removes the newline character from the end of a string.  
=========================================\*/  
  
/\*======== char\* trim() ==========  
Inputs: char \* c  
Returns: Pointer to beginning of string (w/o spaces)  
  
Removes the spaces in front of and behind a string.  
\*c removes spaces from the front.  
\*e removes from back, and ensures null termination.  
isspace() function used to check if pointing to space.  
================================\*/  
  
/\*======== int check_special(char \* cmd) ========  
Inputs: char \* cmd  
Returns: int id# representing what type of special command  
  
Checks if the cmdline input involves redirection or piping.  
Returns an integer identifier based on special character found:  
\> : 1; < : 2; | : 3  
Returns 0 if no pipes or redirection characters found.  
===============================================\*/  
  
/\*======== int size(char \*\* args) ==========  
Inputs: char \*\* args  
Returns: int size  
  
Finds the size of an array of strings.  
==========================================\*/  
  
/\*======== void pipredir(int id, char \* cmd, char \* exec) ==========  
Inputs: int id, char \* cmd, char \* exec  
Returns: NONE  
  
Executes the redirection and piping functionality of the shell.  
id == 1: signals < redirection  
Parses arguments with < delimiter  
Chaining - if more than one '<', only the last one is read as stdin like bash. If there are '>' afterwards, only the  file after the last one will get written to.  
id == 2: signals > redirection  
Parses arguments with > delimiter  
Chaining - if more than one '>', only the last one gets written in like bash.  
id == 3: signals | redirection  
Parses arguments with | delimiter  
Consecutive piping not implemented.  
=====================================================\*/  
  
/\*======== void fork_exec(char \*\* args) ==========  
Inputs: char \* str  
Returns: NONE  
  
Takes an array of arguments and executes them using execvp OR  
if exit command entered: exit the shell  
if cd command entered: use chdir() to move into another directory  
All other commands: fork off process and execvp.  
[Piping and Redirection accounted for in pipredir() fxn above ^^]  
================================================\*/  
  
/\*======== void exec_all(char \* input) ==========  
Inputs: char \* input  
Returns: NONE  
  
Takes in input from the user and calls functions defined above  
based on the situation: e.g., if '<' character exists, then  
call the function that deals with output redirection.  
===============================================*/  
  
`shell.c`  
**Handles the sequence of running commands in the shell, by calling the functions in `fxn.c`**  

