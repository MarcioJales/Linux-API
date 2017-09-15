| Name | Assignment |
| ---- | ---------- |
| memalloc.c | Exercise 2 from chapter 7 |

## Ex 2. - The "malloc" and "free" implementation

This is not intended to be a complete solution.

- The functions were named "heapAlloc" and "heapFree" so that they cannot be confused with native malloc implementations.
- Program break will never descrease.
- Heap is allocated only in ALLOC_THRESHOLD chuncks.
- 2 separated double-linked lists are created for the allocated and free blocks lists. By now, the program behaves as the lists were single-linked lists.
