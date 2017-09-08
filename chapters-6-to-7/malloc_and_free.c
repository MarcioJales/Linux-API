/* Chapter 7, exercise 2
 *
 * (Advanced) Implement malloc() and free().
 */

/* Test program to test malloc and free implementation. Similar to free_and_sbrk.c.
 * ./malloc_and_free num_allocs blk_size [free_step [free_min [free_max]]]
 *
 * Compile with '-D DEBUG' to show program breaks
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memalloc.h"


#ifdef DEBUG
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x, ...) do {} while (0)
#endif


#define MAX_ALLOCS            1000000


int main(int argc, char** argv)
{
  char* endptr;
  int free_step, free_min, free_max, blk_size, num_allocs;
  int j = 1;
  long int args[5];

  if (argc < 3) {
    printf("%s num-allocs block-size [step [block-min [block-max]]]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  while(j < argc) {
    args[j-1] = strtol(argv[j], &endptr, 10);
    if(*endptr != 0) {
      fprintf(stderr, "Invalid numeric value\n");
      exit(EXIT_FAILURE);
    }
    j++;
  }

  num_allocs = args[0];
  if(num_allocs > MAX_ALLOCS)
    fprintf(stderr, "%d max allocations\n", MAX_ALLOCS);

  blk_size = args[1];

  free_step = (argc > 3) ? args[2] : 1;
  free_min = (argc > 4) ? args[3] : 1;
  free_max = (argc > 5) ? args[4] : num_allocs;

  DEBUG_PRINT("Initial program break: %p\n", sbrk(0));

  return 0;
}
