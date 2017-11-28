#include "head.h"

//Reads the input from stdin
char * read_line() {
  char *line = NULL;
  size_t size = 0;
  //if failed to read line
  if ( getline(&line, &size, stdin) == -1 ) {
    free(line);
    printf("exit\n");
    exit(0);
  }
  return line;
}

//Parse arguments of received cmd
char ** parse_args( char * line, char * delim){
  int size = 6; // start with 5 args
  char **args = malloc( size * sizeof(char *));
  int n = 0;
  while( line ){
    args[n] = strsep( &line, delim);
    //dynamic sizing
    if ( ++n == size) {
      size += 5; // add 5 args as necessary
      args = realloc( args, size * sizeof(char *));
    }
  }
  args[n] = 0;
  return args;
}

//Get rid of that pesky newline!!
void strip_newline( char *str ) {
  *strrchr(str, '\n') = 0;
}

//Trim of spaces at ends of cmd
char * trim(char *c) {
    char * e = c + strlen(c) - 1;
    while(*c && isspace(*c)) c++;
    while(e > c && isspace(*e)) *e-- = '\0';
    return c;
}

//Execute commands (call fxns defined above)
void exec_all( char * input ) {
  strip_newline(input);
  char ** cmds = parse_args(input, ";");
  char *cmd = *cmds;
  int n = 0;
  while( cmd ){
    cmd = trim(cmd);
    char **args = parse_args(cmd, " ");
    fork_exec( args );
    cmd = cmds[++n];
    free(args);
  }
  free(cmds);
}

//Forks off a child process to execute cmds
void fork_exec( char ** args ) {
  //if nothing entered
  if( ! strcmp(args[0],"") ) {
    return;
  }
  //if exit command entered
  if (!strcmp(args[0], "exit")) {
    exit(0);
  }
  //if asked to change directory (cd)
  else if ( !strcmp( args[0], "cd" ) ) {
    if (!args[1]) {
      struct passwd *pw = getpwuid(getuid());
      chdir(pw->pw_dir);
    }
    //if unable, print why could not cd into the dir.
    else {
      if (chdir(args[1])) {
        printf("shell: cd: %s: %s\n", args[1], strerror(errno));
      }
    }
  }
  //other commands
  else {
    //if parent process
    if (fork()){
      wait(0);
    }
    //if child process
    else{
      if (execvp(args[0], args)) {
        //only enters if the command isn't valid
        printf("%s: command not found\n", args[0]);
      }
      exit(0);
    }
  }
}

//prints the header for the shell (what you would see on your own)
void print_shell_prompt() {
  char user[256], host[256], wd[256];
  struct passwd *pw = getpwuid(getuid());
  strcpy(user, pw->pw_name);
  gethostname(host, sizeof(host));
  getcwd(wd, sizeof(wd));
  printf("%s@%s:%s$ ", user, host, wd);
}
