#include "head.h"

char ** parse_args( char * line )
{
  char ** args = (char **)malloc(6 * sizeof(char *));
  int i = 0;
  while( (args[i++] = strsep(&line, " ")) );
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
