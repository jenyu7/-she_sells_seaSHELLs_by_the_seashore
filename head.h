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

char * read_line();
char ** parse_args( char * line, char * delim );
void strip_newline( char *str );
void fork_exec( char ** args );
void exec_all( char * input );
void print_shell_prompt();
