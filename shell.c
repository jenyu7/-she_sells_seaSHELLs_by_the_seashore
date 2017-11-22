/*
 * Adeebur Rahman and Jen Yu
 * Shell-ing out commands
 * Systems Pd 10
 * 2017-11-21
 */

#include "head.h"

int main()
{
  char * end = (char *)malloc(256 * sizeof(char));
  char * s;
  fgets(s, sizeof(s), stdin);
  s = strsep(&end, "\n");
  int status;
  char ** cmd = parse_args(s);
  if (fork())
    {
      wait(&status);
      printf("child done doing thing\n");
    }
  else
    {
      printf("doing thang\n");
      execvp(cmd[0], cmd);
    }
  return 0;
}
