#ifndef _MEMALLOC_H
#define _MEMALLOC_H

/* Blocks are allocated/freed only in ALLOC_BLKSIZE chuncks (~= 128 KB).
   Considering a 4096 bytes page size, 135168 equal to 32 pages.
 */
#define ALLOC_THRESHOLD       135168

void* heapAlloc(size_t size);
void heapFree(void* ptr);

#endif
