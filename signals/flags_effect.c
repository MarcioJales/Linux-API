/* Chapter 20, exercise 3
**
** Write programs that verify the effect of the SA_RESETHAND and SA_NODEFER flags when establishing a signal handler with sigaction().
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
  if(argc != 2) {
    fprintf(stderr, "Provide 1 argument, either SA_RESETHAND or SA_NODEFER\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}