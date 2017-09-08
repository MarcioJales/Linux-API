#ifndef _MEMALLOC_H
#define _MEMALLOC_H

/* Blocks are allocated/freed only in ALLOC_BLKSIZE chuncks (128 KB).
 * We may allocate only 2²⁰ blocks */
#define ALLOC_THRESHOLD       131072
#define MAX_NUMBLKS           1048576

void* heapAlloc(size_t size);
void heapFree(void* ptr);

#endif
