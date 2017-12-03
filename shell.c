/*
 * Adeebur Rahman and Jen Yu
 * P01: Shell-ing out commands
 * Systems Pd 10
 * 2017-11-21
 */

#include "head.h"

//handle those signals
static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n");
    print_shell_prompt();
    fflush(stdout);
  }
}

int main()
{
  signal(SIGINT, sighandler);
  while(1) {
    print_shell_prompt();
    //printf("reading line\n");
    char * input = read_line();
    exec_all( input );
    free(input);
    
    //printf("shell\n");
  }
  return 0;
}
