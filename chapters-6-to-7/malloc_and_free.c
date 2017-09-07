/* Chapter 7, exercise 2
 *
 * (Advanced) Implement malloc() and free().
 */

/*
 * ./malloc_and_free num_allocs blk_size [free_step [free_min [free_max]]]
 *
 * Compile with '-D DEBUG' to show program breaks
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef DEBUG
#define DEBUG_PRINT(x) printf(x)
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif


/* free() calls brk() only if block to be released is greater than 128kB */
#define FREE_MIN_BLOCKSIZE    131072
#define MAX_ALLOCS            1000000


struct free_blk {
  long int length;
  struct free_blk* prev_blk;
  struct free_blk* next_blk;
};

int main(int argc, char** argv)
{
  char* endptr;
  int free_step, free_min, free_max, blk_size, num_allocs;
  int j = 1;
  long int args[5];

  if (argc < 3) {
    printf("%s num-allocs block-size [step [block-min [block-max [debug]]]]\n", argv[0]);
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

  return 0;
}
