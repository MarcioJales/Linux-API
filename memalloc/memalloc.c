/* Chapter 7, exercise 2
 *
 * (Advanced) Implement malloc() and free().
 */

/* This is not intended to be a complete malloc solution
 *
 * General informations:
 * The functions were named "heapAlloc" and "heapFree" so that they cannot be confused with complete malloc implementations
 * Program break will never descrease.
 * Heap is allocated/freed only in ALLOC_THRESHOLD chuncks.
 * 2 separated double-linked lists are created for the allocated and free blocks lists.
 * By now, the program behaves as the lists were single-linked lists.
 *
 * **** ON PROGRESS, but already functional ****
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "memalloc.h"

#define FREE_LIST   0
#define ALLOC_LIST  1


struct blockheader {
  long int length;
  struct blockheader *prevblk;
  struct blockheader *nextblk;
};

static struct blockheader *allocdlist = NULL;
static struct blockheader *freelist = NULL;

/* ---------------------------------------------------------------------------------------------*/
static struct blockheader * searchAtFreeList(size_t size)
{
  if(freelist == NULL)
    return NULL;

  /* First-fit */
  struct blockheader *currentblk = freelist;

  while(currentblk != NULL) {
    if(size <= currentblk->length)
      return currentblk;
    else
      currentblk = currentblk->nextblk;
  }
  return NULL;
}

/* ---------------------------------------------------------------------------------------------*/
static struct blockheader * searchAtAllocdList(void *ptr)
{
  if(allocdlist == NULL)
    return NULL;

  struct blockheader *currentblk = allocdlist;

  while(currentblk != NULL) {
    if((struct blockheader *) ((uint8_t *) ptr - sizeof(struct blockheader)) == currentblk)
      return currentblk;
    else
      currentblk = currentblk->nextblk;
  }
  return NULL;
}

/* ---------------------------------------------------------------------------------------------*/
static void removeFromList(struct blockheader *addr, size_t size, uint8_t whichlist)
{
  struct blockheader **list;
  struct blockheader *head;

  if(whichlist == ALLOC_LIST)
    list = &allocdlist;
  if(whichlist == FREE_LIST)
    list = &freelist;

  head = *list;

  while(*list != addr)
    *list = (*list)->nextblk;

  /* blocks from allocdlist will always enter this branch */
  if((*list)->length == size) {
    if(*list == head) {
      *list = (*list)->nextblk;
      (*list)->prevblk = NULL;
      return;
    }

    if((*list)->prevblk != NULL)
      ((*list)->prevblk)->nextblk = (*list)->nextblk;
    if((*list)->nextblk != NULL)
      ((*list)->nextblk)->prevblk= (*list)->prevblk;

    *list = head;
  }
  else {
    *list = (struct blockheader *) ((uint8_t *) addr + size);
    (*list)->length = addr->length - size;
    (*list)->prevblk = addr->prevblk;
    (*list)->nextblk = addr->nextblk;

    if((*list)->nextblk != NULL)
      ((*list)->nextblk)->prevblk = *list;
    if((*list)->prevblk != NULL) {
      ((*list)->prevblk)->nextblk = *list;
      *list = head; /* the list must have the head adjusted since we alter the lists by reference */
    }
  }
}

/* ---------------------------------------------------------------------------------------------*/
static void addOnList(void *addr, size_t size, uint8_t whichlist)
{
  struct blockheader **list;
  struct blockheader *head;

  if(whichlist == ALLOC_LIST)
    list = &allocdlist;
  if(whichlist == FREE_LIST)
    list = &freelist;

  head = *list;

  if(*list == NULL) {
    *list = addr;
    (*list)->length = size;
    (*list)->prevblk = NULL;
    (*list)->nextblk = NULL;
  }
  else
  {
    while(1)
    {
      if(whichlist == FREE_LIST)
      {
        /* If the block freed can be coalesced wether to previous or next block */
        if((uint8_t *) *list + (*list)->length == addr) {
          (*list)->length += size;
          freelist = head;
          return;
        }
        else if((uint8_t *) *list - size /*addr->length*/ == addr)
        {
          ((struct blockheader *) addr)->length += (*list)->length;
          ((struct blockheader *) addr)->nextblk = (*list)->nextblk;
          ((struct blockheader *) addr)->prevblk = (*list)->prevblk;
          *list = addr;

          if((*list)->nextblk != NULL)
            ((*list)->nextblk)->prevblk = *list;
          if((*list)->prevblk != NULL) {
            ((*list)->prevblk)->nextblk = *list;
            freelist = head;
          }
          return;
        }
      }
      if((*list)->nextblk != NULL)
        *list = (*list)->nextblk;
      else
        break;
    }

    (*list)->nextblk = addr;
    ((*list)->nextblk)->length = size;
    ((*list)->nextblk)->prevblk = *list;
    ((*list)->nextblk)->nextblk = NULL;

    *list = head;
  }
}

/* ---------------------------------------------------------------------------------------------*/
void* heapAlloc(size_t size)
{
  long int bytesalloc;
  void *oldbrk = NULL;
  struct blockheader *newblk;

  if(!size)
    return NULL;

  /* We need at least 24 bytes (sizeof(struct blockheader)) to store blockheader. */
  size += sizeof(struct blockheader);

  newblk = searchAtFreeList(size);
  while(newblk == NULL)
  {
    bytesalloc = ALLOC_THRESHOLD;

    oldbrk = sbrk(ALLOC_THRESHOLD);
    if(oldbrk == (void *) -1)
      return NULL;

    addOnList(oldbrk, bytesalloc, FREE_LIST);
    newblk = searchAtFreeList(size);
  }

  removeFromList(newblk, size, FREE_LIST);
  addOnList(newblk, size, ALLOC_LIST);

  return newblk + 1;
}

/* ---------------------------------------------------------------------------------------------*/
void heapFree(void *ptr)
{
  struct blockheader *rmblk;

  if(ptr == NULL)
    return;

  rmblk = searchAtAllocdList(ptr);
  if(rmblk == NULL) {
    fprintf(stderr, "ABORTING: Address %p is not currently allocated \n", ptr);
    exit(EXIT_FAILURE);
  }

  removeFromList(rmblk, rmblk->length, ALLOC_LIST);
  addOnList(rmblk, rmblk->length, FREE_LIST);
}

/* ---------------------------------------------------------------------------------------------*/
static void printList()
{
  struct blockheader *head = freelist;

  int i = 0;
  while(freelist != NULL) {
    printf("[%d] freelist = %p\n", i, freelist);
    printf("[%d] freelist->length = %ld\n", i, freelist->length);
    printf("[%d] freelist->prevblk = %p\n", i, freelist->prevblk);
    printf("[%d] freelist->nextblk = %p\n", i, freelist->nextblk);
    freelist = freelist->nextblk;
    i++;
  }
  freelist = head;
  printf("\n");
  head = allocdlist;
  i = 0;
  while(allocdlist != NULL) {
    printf("[%d] allocdlist = %p\n", i, allocdlist);
    printf("[%d] allocdlist->length = %ld\n", i, allocdlist->length);
    printf("[%d] allocdlist->prevblk = %p\n", i, allocdlist->prevblk);
    printf("[%d] allocdlist->nextblk = %p\n", i, allocdlist->nextblk);
    allocdlist = allocdlist->nextblk;
    i++;
  }
  allocdlist = head;
}*/

/* --------------------------------------------------------------------------------------------- */
int main()
{
  char *str[128];
  printf("\n\n");

  str[0] = (char*) heapAlloc(9000);
  printf("str[0] = heapAlloc(9000) = %p\n", str[0]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[1] = (char*) heapAlloc(4608);
  printf("str[1] = heapAlloc(4608) = %p\n", str[1]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[2] = (char*) heapAlloc(512);
  printf("str[2] = heapAlloc(512) = %p\n", str[2]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[3] = (char*) heapAlloc(512);
  printf("str[3] = heapAlloc(512) = %p\n", str[3]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[4] = (char*) heapAlloc(256);
  printf("str[4] = heapAlloc(256) = %p\n", str[4]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[5] = (char*) heapAlloc(512);
  printf("str[5] = heapAlloc(512) = %p\n", str[5]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[6] = (char*) heapAlloc(128);
  printf("str[6] = heapAlloc(128) = %p\n", str[6]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[7] = (char*) heapAlloc(900);
  printf("str[7] = heapAlloc(900) = %p\n", str[7]);
  printf("\n\n");
  printList();
  printf("\n\n");
  heapFree(str[2]);
  printf("heapFree(str[2])\n");
  printf("\n\n");
  printList();
  printf("\n\n");
  heapFree(str[4]);
  printf("heapFree(str[4])\n");
  printf("\n\n");
  printList();
  printf("\n\n");
  heapFree(str[5]);
  printf("heapFree(str[5])\n");
  printf("\n\n");
  printList();
  printf("\n\n");
  heapFree(str[3]);
  printf("heapFree(str[3])\n");
  printf("\n\n");
  printList();
  printf("\n\n");
  heapFree(str[6]);
  printf("heapFree(str[6])\n");
  printf("\n\n");
  printList();
  printf("\n\n");
  str[8] = (char*) heapAlloc(3060);
  printf("str[8] = heapAlloc(3060) = %p\n", str[8]);
  printf("\n\n");
  printList();
  printf("\n\n");
  str[9] = (char*) heapAlloc(900);
  printf("str[9] = heapAlloc(900) = %p\n", str[9]);
  printf("\n\n");
  printList();
  printf("\n\n");
  heapFree(str[8]);
  printf("heapFree(str[8])\n");
  printf("\n\n");
  printList();
  printf("\n\n");
  str[10] = (char*) heapAlloc(2030);
  printf("str[10] = heapAlloc(2030) = %p\n", str[10]);
  printf("\n\n");
  printList();
  printf("\n\n");
  heapFree(str[9]);
  printf("heapFree(str[9])\n");
  printf("\n\n");
  printList();
  printf("\n\n");
  //Test working until here

  return 0;
}
