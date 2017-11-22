#include "head.h"

char ** parse_args( char * line )
{
  char ** args = (char **)malloc(6 * sizeof(char *));
  int i = 0;
  while( (args[i++] = strsep(&line, " ")) );
  return args;
}
