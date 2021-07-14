/* Chapter 20, exercise 3
**
** Write programs that verify the effect of the SA_RESETHAND and SA_NODEFER flags when establishing a signal handler with sigaction().
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
  if(argc != 2) {
    fprintf(stderr, "Provide 1 argument, either SA_RESETHAND or SA_NODEFER\n");
    exit(EXIT_FAILURE);
  }

  if(!strcmp(argv[1], "SA_RESETHAND")) {
    fprintf(stdout, "SA_RESETHAND\n");
  }
  else if(!strcmp(argv[1], "SA_NODEFER")) {
    fprintf(stdout, "SA_NODEFER\n");
  }
  else {
    fprintf(stderr, "Please, provide either SA_RESETHAND or SA_NODEFER\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}