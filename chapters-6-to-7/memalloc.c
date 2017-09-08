/* Chapter 7, exercise 2
 *
 * (Advanced) Implement malloc() and free().
 */

/* This is not intended to be a complete malloc solution
 *
 * General informations:
 * The functions were named "heapAlloc" and "heapFree" so that they cannot be confused with complete malloc implementations
 * Program break will never become less than the half of the maximum bytes allocated.
 * Heap is allocated/freed only in ALLOC_THRESHOLD chuncks.
 * We may allocate only 2²⁰ blocks.
 *
 * **** ON PROGRESS ****
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "memalloc.h"


struct free_blk_list {
  long int length;
  struct free_blk_list* prevblk;
  struct free_blk_list* nextblk;
};

static long int *memblk[MAX_NUMBLKS];
static long int index = 0, blks_allocd = 0;
static struct free_blk_list* freelist = NULL;

static void searchFreeBlk()
{

}

void* heapAlloc(size_t size)
{
  long int num_to_alloc;
  uint8_t *prev_brk = NULL;

  num_to_alloc = size/ALLOC_THRESHOLD + 1;
  if(num_to_alloc > blks_allocd) {
    prev_brk = (uint8_t *) sbrk(num_to_alloc * ALLOC_THRESHOLD);
    if(*prev_brk == -1)
      return NULL;

    blks_allocd += num_to_alloc;
  }

  /* We need at least 24 bytes to store free_blk_list when a space is dealocated.
     "long int" is 8 bytes long already.
   */
  if(size < 24 - sizeof(long int))
    size = 24 - sizeof(long int);

  if(freelist == NULL) {
    memblk[index] = (long int *) prev_brk;
    *memblk[index] = size;
    freelist = (struct free_blk_list*) ((uint8_t*) (memblk[index] + 1) + size);
    freelist->length = blks_allocd * ALLOC_THRESHOLD - *memblk[index];
    freelist->prevblk = NULL;
    freelist->nextblk = NULL;
  }
  else
    searchFreeBlk();

  index++;

  return (void *) memblk[index-1] + 1;
}

void heapFree(void *ptr)
{

}

int main()
{
  char *str, *str2;
  str = (char*) heapAlloc(4);

  return 0;
}

/* printf("%p, %p, %p\n", memblk[index], memblk[index]+1, freelist); */
