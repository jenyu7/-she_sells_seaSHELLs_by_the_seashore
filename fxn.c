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

//returns 1 for stdout redirection (>), 2 for stdin redirection (<), or 0
int check_special(char * cmd) {
  if(strchr(cmd, '>')) return 1;
  if(strchr(cmd, '<')) return 2;
  if(strchr(cmd, '|')) return 3;
  return 0;
}

//handles commands that require redirection
void pipredir(int id, char * cmd) {
  char ** args;
  char ** new_cmd;
  int new, copy, old;
  if (id == 1){
    args = parse_args(cmd, ">");
    if (strcmp(args[1], "")) {
      new = open(trim(args[1]), O_CREAT | O_WRONLY, 0644);
      copy = dup(STDOUT_FILENO);
      old = dup2(new, STDOUT_FILENO);
    }
    else {
      printf("shell: syntax error near >\n");
      free(args);
      return;
    }
  }
  else if (id == 2) {
    args = parse_args(cmd, "<");
    if (strcmp(args[1], "")) {
      new = open(trim(args[1]), O_RDONLY);
      copy = dup(STDIN_FILENO);
      old = dup2(new, STDIN_FILENO);
    }
    else {
      printf("shell: syntax error near <\n");
      free(args);
      return;
    }
  }
  else if (id == 3){
    args = parse_args(cmd, "|");
    if (strcmp(args[1], "")
      {
	FILE *fp;
	char path[128];
	fp = popen(args[0],"w");
	if (fp = NULL)
	  {
	    printf("bleh\n");
	    return;
	  }
	copy = dup(STDIN_FILENO);
	old = dup2(fileno(fp), STDIN_FILENO);
	new_cmd = parse_args(trim(args[1]), " ");
	fork_exec(new_cmd);
	dup2(copy, old);
	pclose(fp);
	free(new_cmd);
	free(args);
	return;
      }
      else
	{
	  printf("Syntax error\n");
	  return;
	}
  }
  new_cmd = parse_args(trim(args[0]), " ");
  fork_exec(new_cmd);
  dup2(copy, old);
  close(new);
  free(new_cmd);
  free(args);
}

//Execute commands (call fxns defined above)
void exec_all( char * input ) {
  strip_newline(input);
  char ** cmds = parse_args(input, ";");
  char *cmd = *cmds;
  int n = 0;
  while( cmd ){
    cmd = trim(cmd);
    if (check_special(cmd)) {
      pipredir(check_special(cmd), cmd);
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
