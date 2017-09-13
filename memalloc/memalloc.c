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

#define FREE_LIST   0
#define ALLOC_LIST  1


struct blk_header {
  long int length;
  struct blk_header *prevblk;
  struct blk_header *nextblk;
};

static struct blk_header *allocdlist = NULL;
static struct blk_header *freelist = NULL;

static struct blk_header * searchAtList(size_t size, uint8_t whichlist)
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

static void removeFromList(struct blk_header *newblk, size_t size, uint8_t whichlist)
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

static void addOnList(void *addr, size_t size, uint8_t whichlist)
{
  struct blk_header **list;

  if(whichlist == ALLOC_LIST)
    list = &allocdlist;
  if(whichlist == FREE_LIST)
    list = &freelist;

  if(*list == NULL) {
    *list = addr;
    (*list)->length = size;
    (*list)->prevblk = NULL;
    (*list)->nextblk = NULL;
  }
  else {
    struct blk_header *head = *list;

    while((*list)->nextblk != NULL)
      *list = (*list)->nextblk;

    /* This is to coalesce free areas */
    if((uint8_t *) *list + (*list)->length == addr && whichlist == FREE_LIST) {
      (*list)->length += size;
      freelist = head;
      return;
    }

    (*list)->nextblk = addr;
    ((*list)->nextblk)->length = size;
    ((*list)->nextblk)->prevblk = *list;
    ((*list)->nextblk)->nextblk = NULL;

    if(whichlist == ALLOC_LIST)
      allocdlist = head;
    if(whichlist == FREE_LIST)
      freelist = head;
  }
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

  new_blk = searchAtList(size, FREE_LIST);
  while(new_blk == NULL) {
    if(freelist == NULL)
      bytes_to_alloc = (size/ALLOC_THRESHOLD + 1) * ALLOC_THRESHOLD;
    else
      bytes_to_alloc = (size/ALLOC_THRESHOLD) * ALLOC_THRESHOLD;

    old_brk = sbrk(bytes_to_alloc);
    if(old_brk == (void *) -1)
      return NULL;

    addOnList(old_brk, bytes_to_alloc, FREE_LIST);
    new_blk = searchAtList(size, FREE_LIST);
  }
  removeFromList(new_blk, size, FREE_LIST);
  addOnList(new_blk, size, ALLOC_LIST);

  return new_blk;
}

void heapFree(void *ptr)
{

}

int main()
{
  char *str1, *str2, *str3;
  printf("\n\n");

  str1 = (char*) heapAlloc(4608);
  printf("heapAlloc(4608) = %p\n", str1);
  str1 = (char*) heapAlloc(4608);
  printf("heapAlloc(4608) = %p\n", str1);
  str2 = (char*) heapAlloc(512);
  printf("heapAlloc(512) = %p\n", str2);
  str3 = (char*) heapAlloc(512);
  printf("heapAlloc(512) = %p\n", str3);
  str3 = (char*) heapAlloc(256);
  printf("heapAlloc(256) = %p\n", str3);

  printf("\n\n");

  return 0;
}
