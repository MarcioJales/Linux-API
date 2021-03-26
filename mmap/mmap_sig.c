/* Chapter 49, exercise 3
 *
 * Write programs to verify that the SIGBUS and SIGSEGV signals are delivered in the circumstances described in Section 49.4.3.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
  char *addr;
  int fd;

  /* Example from figure 49-3 */
  /* File is 6000 bytes. Maps 6000 bytes. Page size multiple is allocated (8192). SIGSEGV after */
  fd = open("case1.txt", O_RDWR);
  addr = mmap(NULL, 6000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  printf("Write 'a' to (addr + 2000): %p\n", addr + 2000);
  *(addr + 2000) = 'a';
  printf("(addr + 2000) = %c\n", *(addr + 2000));

  printf("Write 'b' to (addr + 7000): %p\n", addr + 7000);
  *(addr + 7000) = 'b';
  printf("(addr + 7000) = %c\n", *(addr + 7000));

  printf("Write 'c' to (addr + 9200): %p\n", addr + 9200);
  *(addr + 9200) = 'c';
  printf("(addr + 9200) = %c\n", *(addr + 9200));

  close(fd);
  munmap(addr, 6000);

  /* Example from figure 49-3 */
  /* File is 2200. Mpas 8192 bytes. Page size multiple is allocated (4096). SIGBUS between 4096 and 8192 */
  fd = open("case2.txt", O_RDWR);
  addr = mmap(NULL, 8192, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  printf("Write 'a' to (addr + 1500): %p\n", addr + 1500);
  *(addr + 1500) = 'a';
  printf("(addr + 1500) = %c\n", *(addr + 1500));

  printf("Write 'b' to (addr + 3600): %p\n", addr + 3600);
  *(addr + 3600) = 'b';
  printf("(addr + 3600)) = %c\n", *(addr + 3600));

  printf("Write 'c' to (addr + 6000): %p\n", addr + 6000);
  *(addr + 6000) = 'c';
  printf("(addr + 6000) = %c\n", *(addr + 6000));

  close(fd);
  munmap(addr, 8192);

  return 0;

}