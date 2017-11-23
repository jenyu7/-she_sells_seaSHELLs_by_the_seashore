/*
 * Adeebur Rahman and Jen Yu
 * Shell-ing out commands
 * Systems Pd 10
 * 2017-11-21
 */

#include "head.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n");
    print_shell_input();
    fflush(stdout);
  }
}

int main()
{
  signal(SIGINT, sighandler);

  while(1) {
    print_shell_input();
    char input[256];
    if ( !fgets(input, sizeof(input), stdin) ) {
      printf("logout");
      exit(0);
    }
    strip_newline( input );
    fork_exec( parse_args(input) );
  }
  return 0;
}
