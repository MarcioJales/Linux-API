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
 * 2 separated double-linked lists are created for the allocated and free blocks lists.
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

static struct blk_header * searchAtFreeList(size_t size)
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
}

static void extendFreeList(void *oldbrk, long int bytes_to_alloc)
{
  if(freelist == NULL) {
    freelist = oldbrk;
    freelist->length = bytes_to_alloc;
    freelist->prevblk = NULL;
    freelist->nextblk = NULL;
  }
  else {
    struct blk_header *headFL = freelist;

    while(freelist->nextblk != NULL)
      freelist = freelist->nextblk;

    if((uint8_t *) freelist + freelist->length == oldbrk)
      freelist->length += bytes_to_alloc;
    else {
      freelist->nextblk = oldbrk;
      (freelist->nextblk)->length = bytes_to_alloc;
      (freelist->nextblk)->prevblk = freelist;
      (freelist->nextblk)->nextblk = NULL;
    }

    freelist = headFL;
  }
}

static void removeFromFreeList(struct blk_header *newblk, size_t size)
{
  struct blk_header *headFL = freelist;

  while(freelist != newblk)
    freelist = freelist->nextblk;

  if(freelist->length == size) {
    if(freelist == headFL) {
      freelist = freelist->nextblk;
      return;
    }

    if(freelist->prevblk != NULL)
      (freelist->prevblk)->nextblk = freelist->nextblk;
    if(freelist->nextblk != NULL)
      (freelist->nextblk)->prevblk= freelist->prevblk;

    freelist = NULL;
    freelist = headFL;
  }
  else {
    freelist = (struct blk_header *) ((uint8_t *) newblk + size);
    freelist->length = newblk->length - size;
    freelist->prevblk = newblk->prevblk;
    freelist->nextblk = newblk->nextblk;

    if(freelist != (struct blk_header *) ((uint8_t *) headFL + size))
      freelist = headFL;
  }
}

static void addOnAllocdList(struct blk_header *newblk, size_t size)
{
  if(allocdlist == NULL) {
    allocdlist = newblk;
    allocdlist->length = size;
    allocdlist->prevblk = NULL;
    allocdlist->nextblk = NULL;
  }
  else {
    struct blk_header *headAL = allocdlist;

    while(allocdlist->nextblk != NULL)
      allocdlist = allocdlist->nextblk;

    allocdlist->nextblk = newblk;
    (allocdlist->nextblk)->length = size;
    (allocdlist->nextblk)->prevblk = allocdlist;
    (allocdlist->nextblk)->nextblk = NULL;

    if(allocdlist != headAL);
      allocdlist = headAL;
  }
}

static void updateLists(struct blk_header *newblk, size_t size)
{
  removeFromFreeList(newblk, size);
  addOnAllocdList(newblk, size);
}

void* heapAlloc(size_t size)
{
  long int bytes_to_alloc;
  void *old_brk = NULL;
  struct blk_header *new_blk;

  if(!size)
    return NULL;

  /* We need at least 24 bytes (sizeof(struct blk_header)) to store blk_header. */
  if(size < sizeof(struct blk_header))
    size = sizeof(struct blk_header);

  new_blk = searchAtFreeList(size);
  while(new_blk == NULL) {
    if(freelist == NULL)
      bytes_to_alloc = (size/ALLOC_THRESHOLD + 1) * ALLOC_THRESHOLD;
    else
      bytes_to_alloc = (size/ALLOC_THRESHOLD) * ALLOC_THRESHOLD;

    old_brk = sbrk(bytes_to_alloc);
    if(old_brk == (void *) -1)
      return NULL;

    extendFreeList(old_brk, bytes_to_alloc);
    new_blk = searchAtFreeList(size);
  }
  updateLists(new_blk, size);

  return new_blk;
}

void heapFree(void *ptr)
{

}

int main()
{
  char *str, *str2;
  printf("\n\n");
  str = (char*) heapAlloc(4096);
  printf("heapAlloc(4096) = %p\n", str);
  str = (char*) heapAlloc(512);
  printf("heapAlloc(512) = %p\n", str);
  str = (char*) heapAlloc(512);
  printf("heapAlloc(512) = %p\n", str);
  printf("\n\n");
  return 0;
}
