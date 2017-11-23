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

char ** parse_args( char * line );
void strip_newline( char *str );
void fork_exec( char ** args );
void exec_all( char * input );
void print_shell_input();
