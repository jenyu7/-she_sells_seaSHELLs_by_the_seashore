#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>

void print_shell_prompt();
char * read_line();
char ** parse_args( char * line, char * delim );
void strip_newline( char *str );
char* trim();
int check_special(char * cmd);
int size(char ** args);
void pipredir(int id, char * cmd, char * exec);
void fork_exec( char ** args );
void exec_all( char * input );
