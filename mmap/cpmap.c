/* Chapter 49, exercise 1
 *
 * Write a program, analogous to cp(1), that uses mmap() and memcpy() calls (instead of read() or write()) to copy a source file to a destination file. (Use fstat() to obtain the size of the input file, which can then be used to size the required memory mappings, and use ftruncate() to set the size of the output file.)
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  if(argc != 3) {
    fprintf(stderr, "Give 2 filenames\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}