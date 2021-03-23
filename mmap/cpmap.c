/* Chapter 49, exercise 1
 *
 * Write a program, analogous to cp(1), that uses mmap() and memcpy() calls (instead of read() or write()) to copy a source file to a destination file. (Use fstat() to obtain the size of the input file, which can then be used to size the required memory mappings, and use ftruncate() to set the size of the output file.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  int fd_input, fd_output;
  struct stat statbuf;
  char *addr;

  if(argc != 3) {
    fprintf(stderr, "Give 2 filenames: input first and output second\n");
    exit(EXIT_FAILURE);
  }

  fd_input = open(argv[1], O_RDONLY);
  if(fd_input == -1) {
    fprintf(stderr, "Error to open input file\n");
    exit(EXIT_FAILURE);
  }

  if(fstat(fd_input, &statbuf) == -1) {
    fprintf(stderr, "Error to get file information\n");
    exit(EXIT_FAILURE);
  }

  addr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd_input, 0);

  /* According to the book: 
  Once mmap() has been called, we can close the file descriptor without affecting the mapping.
  */
  if(close(fd_input) == -1) {
    fprintf(stderr, "Failed to close input file\n");
    exit(EXIT_FAILURE);
  }

  fd_output = open(argv[1], O_WRONLY | O_CREAT);
  if(fd_output == -1) {
    fprintf(stderr, "Error to open output file\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}