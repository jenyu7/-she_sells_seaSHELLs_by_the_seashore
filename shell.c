/*
 * Adeebur Rahman and Jen Yu
 * P01: Shell-ing out commands
 * Systems Pd 10
 * 2017-11-21
 */

#include "head.h"

/*========static void sighandler(int signo)========
  Inputs: int signo
  Outputs: NONE

  If Ctrl-C pressed, print out a new shell prompt.
  ===============================================*/
static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n");
    print_shell_prompt();
    fflush(stdout);
  }
}

/*========int main()========
  Inputs: NONE
  Outputs: 0

  Keeps the shell running! 
  Prints prompt, reads input, and executes fxns.
  =========================*/
int main()
{
  signal(SIGINT, sighandler);
  while(1) {
    print_shell_prompt();
    char * input = read_line();
    exec_all( input );
    free(input);
  }
  return 0;
}
