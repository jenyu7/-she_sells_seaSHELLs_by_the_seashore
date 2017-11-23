#include "head.h"

char * read_line() {
  char *line = NULL;
  size_t size = 0;
  if ( getline(&line, &size, stdin) == -1 ) {
    printf("exit\n");
    exit(0);
  }
  return line;
}

char ** parse_args( char * line, char delim){
  int size = 6; // start with 5 args
  char **args = malloc( size * sizeof(char *));
  int n = 0;
  while( line ){
    args[n] = strsep( &line, &delim);
    if ( ++n == size) {
      size += 5; // add 5 args as necessary
      args = realloc( args, size * sizeof(char *));
    }
  }
  args[n] = 0;
  return args;
}

void strip_newline( char *str ) {
  while( *str ) {
    if( *str == '\n' ) {
      *str = 0;
    }
    str++;
  }
}

char * trim(char *c) {
    char * e = c + strlen(c) - 1;
    while(*c && isspace(*c)) c++;
    while(e > c && isspace(*e)) *e-- = '\0';
    return c;
}

void exec_all( char * input ) {
  char ** cmds = parse_args(input, ';');
  char *cmd = *cmds;
  int n = 0;
  while( cmd ){
    cmd = trim(cmd);
    char **args = parse_args(cmd, ' ');
    fork_exec( args );
    cmd = cmds[++n];
  }
}


void fork_exec( char ** args ) {
  if (!strcmp(args[0], "exit")) {
    exit(0);
  }
  else if ( !strcmp( args[0], "cd" ) ) {
    if (!args[1]) {
      struct passwd *pw = getpwuid(getuid());
      chdir(pw->pw_dir);
    }
    else {
      if (chdir(args[1])) {
        printf("shell: cd: %s: %s\n", args[1], strerror(errno));
      }
    }
  }
  else {
    if (fork()){
      wait(0);
    }
    else{
      if (execvp(args[0], args)) {
        printf("%s: command not found\n", args[0]);
      }
      exit(0);
    }
  }
}

void print_shell_input() {
  char user[15], host[15], wd[50];
  struct passwd *pw = getpwuid(getuid());
  strcpy(user, pw->pw_name);
  gethostname(host, sizeof(host));
  getcwd(wd, sizeof(wd));
  printf("%s@%s:%s$ ", user, host, wd);
}
