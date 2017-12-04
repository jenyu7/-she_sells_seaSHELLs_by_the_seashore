#include "head.h"

/*======== void print_shell_prompt() ==========
  Inputs: NONE
  Returns: NONE

  Prints out the shell prompt of the user, as would normally
  be seen in bash.
  =============================================*/
void print_shell_prompt() {
  char user[256], host[256], wd[256];
  struct passwd *pw = getpwuid(getuid());
  strcpy(user, pw->pw_name);
  gethostname(host, sizeof(host));
  getcwd(wd, sizeof(wd));
  printf("%s@%s:%s$ ", user, host, wd);
}

/*======== char * read_line() ==========
  Inputs: NONE
  Returns: Line read from stdin
  Otherwise, exits.
  Uses C function getline(char **lineptr, size_t *n, FILE *stream) [man 3]
  Tries to read line from stdin,
  If failed, exits program.
  =======================================*/
char * read_line() {
  char *line = NULL;
  size_t size = 0;
  //if failed to read line
  if (getline(&line, &size, stdin) == -1 ) {
    free(line);
    printf("exit\n");
    exit(0);
  }
  return line;
}

/*======== char ** parse_args() ==========
  Inputs: char * line, char * delim
  Returns: char ** [array of strings previously separated by delim]

  Starts with parsing five arguments using strsep
  If more than five args, array dynamically resized with +5 more arguments
  =========================================*/
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

/*======== void strip_newline() ==========
  Inputs: char * str
  Returns: NONE

  Removes the newline character from the end of a string.
  =========================================*/
void strip_newline( char *str ) {
  *strrchr(str, '\n') = 0;
}

/*======== char* trim() ==========
  Inputs: char * c
  Returns: Pointer to beginning of string (w/o spaces)

  Removes the spaces in front of and behind a string.
  *c removes spaces from the front.
  *e removes from back, and ensures null termination.
  isspace() function used to check if pointing to space.
  ================================*/
char * trim(char *c) {
  char * e = c + strlen(c) - 1;
  while(*c && isspace(*c)) c++;
  while(e > c && isspace(*e)) *e-- = '\0';
  return c;
}

/*======== int check_special(char * cmd) ========
  Inputs: char * cmd
  Returns: int id# representing what type of special command

  Checks if the cmdline input involves redirection or piping.
  Returns an integer identifier based on special character found:
  | : 1, < : 2; > : 3;
  Returns 0 if no pipes or redirection characters found.
  Returns -1 if >>, >&, >>&, 1>, 1>>, 2>, 2>>, &>, or &>> used (not supported)
  ===============================================*/
int check_special(char * cmd) {
  if(strstr(cmd, ">>")) return -1;
  if(strstr(cmd, ">&")) return -1;
  if(strstr(cmd, "1>")) return -1;
  if(strstr(cmd, "2>")) return -1;
  if(strstr(cmd, "&>")) return -1;
  if(strchr(cmd, '|')) return 1;
  if(strchr(cmd, '<')) return 2;
  if(strchr(cmd, '>')) return 3;
  return 0;
}

/*======== int size(char ** args) ==========
  Inputs: char ** args
  Returns: int size

  Finds the size of an array of strings.
  ==========================================*/
int size(char** args) {
  int i = 0;
  while(*args++) { i++; }
  return i;
}


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
void pipredir(int id, char * cmd, char * exec) {
  int new, copy, old;
  if (id == -1) {
    printf("shell: error this form of redirect is not supported\n");
  }
  else if (id == 1) {
    char back[strlen(cmd)];
    strcpy(back, cmd);
    strsep(&cmd, "|");
    if (check_special(cmd)) {
      printf("shell: error piping can not be combined with other redirects\n");
      return;
    }
    char ** args = parse_args(back, "|");
    if (strcmp(args[1], "")) {
      FILE *fp;
      fp = popen(args[0],"r");
      if (!fp) {
          printf("shell: error pipe could not be created\n");
          return;
      }
      copy = dup(STDIN_FILENO);
      old = dup2(fileno(fp), STDIN_FILENO);
      char ** new_cmd = parse_args(trim(args[1]), " ");
      fork_exec(new_cmd);
      dup2(copy, old);
      pclose(fp);
      free(new_cmd);
      free(args);
    }
    else {
      printf("shell: syntax error near |\n");
    }
  }
  else if (id == 2) {
    if (exec) { strsep(&cmd, "<"); }
    else { exec = strsep(&cmd, "<"); }
    if (!strcmp(cmd, "")) {
      printf("shell: syntax error near <\n");
      return;
    }
    if (check_special(cmd)) {
      cmd = trim(cmd);
      char back[strlen(cmd)];
      strcpy(back, cmd);
      char * curfile = strsep(&cmd, " ");
      curfile = trim(curfile);
      new = open(curfile, O_RDONLY);
      copy = dup(STDIN_FILENO);
      old = dup2(new, STDIN_FILENO);
      pipredir(check_special(back), back, exec);
      dup2(copy, old);
    }
    else {
      cmd = trim(cmd);
      new = open(cmd, O_RDONLY);
      if (new == -1) {
        printf("shell: '%s': No such file or directory\n", cmd);
        close(new);
        return;
      }
      copy = dup(STDIN_FILENO);
      old = dup2(new, STDIN_FILENO);
      exec = trim(exec);
      char ** args = parse_args(exec, " ");
      fork_exec(args);
      free(args);
      dup2(copy, old);
      close(new);
    }
  }
  else if (id == 3) {
    if (exec) { strsep(&cmd, ">"); }
    else { exec = strsep(&cmd, ">"); }
    if (!strcmp(cmd, "")) {
      printf("shell: syntax error near >\n");
      return;
    }
    if (check_special(cmd)) {
      cmd = trim(cmd);
      char back[strlen(cmd)];
      strcpy(back, cmd);
      char * curfile = strsep(&cmd, " ");
      curfile = trim(curfile);
      new = open(curfile, O_CREAT | O_WRONLY, 0644);
      copy = dup(STDOUT_FILENO);
      old = dup2(new, STDOUT_FILENO);
      pipredir(check_special(back), back, exec);
      dup2(copy, old);
    }
    else {
      new = open(trim(cmd), O_CREAT | O_WRONLY, 0644);
      copy = dup(STDOUT_FILENO);
      old = dup2(new, STDOUT_FILENO);
      exec = trim(exec);
      char ** args = parse_args(exec, " ");
      fork_exec(args);
      free(args);
      dup2(copy, old);
      close(new);
    }
  }
}

/*======== void fork_exec(char ** args) ==========
  Inputs: char * str
  Returns: NONE

  Takes an array of arguments and executes them using execvp OR
  if exit command entered: exit the shell
  if cd command entered: use chdir() to move into another directory
  All other commands: fork off process and execvp.
  [Piping and Redirection accounted for in pipredir() fxn above ^^]
  ================================================*/
void fork_exec( char ** args ) {
  //if nothing entered
  if( ! strcmp(args[0],"") ) {
    return;
  }
  //if exit command entered
  if (!strcmp(args[0], "exit")) {
    free(args);
    exit(0);
  }
  //if asked to change directory (cd)
  else if ( !strcmp( args[0], "cd" ) ) {
    if (args[1]) {
      if (chdir(args[1])) {
        //if unable, print why could not cd into the dir.
        printf("shell: cd: %s: %s\n", args[1], strerror(errno));
      }
    }
    else {
      struct passwd *pw = getpwuid(getuid());
      chdir(pw->pw_dir);
    }
  }
  //other commands
  else {
    //if parent process
    int cpid;
    if ((cpid = fork())){
      waitpid(cpid, NULL, 0);
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

/*======== void exec_all(char * input) ==========
  Inputs: char * input
  Returns: NONE

  Takes in input from the user and calls functions defined above
  based on the situation: e.g., if '<' character exists, then
  call the function that deals with output redirection.
  ===============================================*/
void exec_all( char * input ) {
  strip_newline(input);
  char ** cmds = parse_args(input, ";");
  char *cmd = *cmds;
  int n = 0;
  while( cmd ){
    cmd = trim(cmd);
    if (check_special(cmd)) {
      pipredir(check_special(cmd), cmd, 0);
    }
    else {
      char **args = parse_args(cmd, " ");
      fork_exec( args );
      free(args);
    }
    cmd = cmds[++n];
  }
  free(cmds);
}
