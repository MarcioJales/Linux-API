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
 * 2 separated double-linked lists will are created for the allocated and free blocks lists.
 *
 * **** ON PROGRESS ****
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "memalloc.h"


struct blk_header {
  long int length;
  struct blk_header *prevblk;
  struct blk_header *nextblk;
};

static struct blk_header *allocdlist = NULL;
static struct blk_header *freelist = NULL;

static struct blk_header * searchFreeBlk(size_t size)
{
  if(freelist == NULL)
    return NULL;
  else {
    /* First-fit */
    struct blk_header *currentblk = freelist;

    while(currentblk != NULL) {
      if(size <= currentblk->length)
        return currentblk;
      else
        currentblk = currentblk->nextblk;
    }
  }
  return NULL;
};

static void addFreeBlk(void *oldbrk, long int bytes_to_alloc)
{
  if(freelist == NULL) {
    freelist = oldbrk;
    freelist->length = bytes_to_alloc;
    freelist->prevblk = NULL;
    freelist->nextblk = NULL;
  }
  else {
    struct blk_header *currentblk = freelist;

    while(currentblk->nextblk != NULL)
      currentblk = currentblk->nextblk;

    if((uint8_t *) currentblk + currentblk->length == oldbrk) {
      currentblk->length += bytes_to_alloc;
    }
    else {
      currentblk->nextblk = oldbrk;
      (currentblk->nextblk)->length = bytes_to_alloc;
      (currentblk->nextblk)->prevblk = currentblk;
      (currentblk->nextblk)->nextblk = NULL;
    }
  }
};

static void updateAllocdList(struct blk_header *newblk, size_t size)
{
  if(allocdlist == NULL) {
    allocdlist = newblk;
    allocdlist->length = size;
    allocdlist->prevblk = NULL;
    allocdlist->nextblk = NULL;
  }
};

void* heapAlloc(size_t size)
{
  long int bytes_to_alloc;
  static int c = 0;
  void *old_brk = NULL;
  struct blk_header *new_blk;

  if(!size)
    return NULL;

  /* We need at least 24 bytes (sizeof(struct blk_header)) to store blk_header. */
  if(size < sizeof(struct blk_header))
    size = sizeof(struct blk_header);

  new_blk = searchFreeBlk(size);
  if(new_blk == NULL) {
      if(freelist == NULL && allocdlist == NULL)
        bytes_to_alloc = (size/ALLOC_THRESHOLD + 1) * ALLOC_THRESHOLD;
      else
        bytes_to_alloc = (size/ALLOC_THRESHOLD) * ALLOC_THRESHOLD;

      old_brk = sbrk(bytes_to_alloc);
      if(old_brk == (void *) -1)
        return NULL;

      addFreeBlk(old_brk, bytes_to_alloc);
      new_blk = searchFreeBlk(size);
      printf("%p, %ld, %p, %p\n", new_blk, new_blk->length, new_blk->prevblk, new_blk->nextblk);
  }
  updateAllocdList(new_blk, size);
  printf("%p, %ld, %p, %p\n", allocdlist, allocdlist->length, allocdlist->prevblk, allocdlist->nextblk);

  return NULL;
}

void heapFree(void *ptr)
{

}

int main()
{
  char *str, *str2;
  printf("\n\n");
  str2 = (char*) heapAlloc(4096);

  return 0;
}

/*

addFreeBlk(old_brk, bytes_to_alloc);
printf("%p\n", freelist);
printf("%p, %ld, %p, %p\n", freelist, freelist->length, freelist->prevblk, freelist->nextblk);

//printf("+ = %p\n", (uint8_t *) currentblk + currentblk->length);
if((uint8_t *) currentblk + currentblk->length == oldbrk) {
  currentblk->length += bytes_to_alloc;
  //printf("cbl = %ld\n", currentblk->length);

printf("now_brk = %p\n", sbrk(0));
old_brk = sbrk(bytes_to_alloc);
printf("bytes_to_alloc = %ld, old_brk = %p, now_brk = %p\n", bytes_to_alloc, old_brk, sbrk(0));

void* myAlloc(size_t size)
{
  static void *old_brk;

  printf("old_brk = %p, now_brk = %p\n", old_brk, sbrk(0));
  old_brk = sbrk(size);
  printf("bytes_to_alloc = %ld, old_brk = %p, now_brk = %p\n", size, old_brk, sbrk(0));

  return NULL;
}*/
