/* Chapter 5, exercise 3
 *
 * This exercise is designed to demonstrate why the atomicity guaranteed by opening a file with the O_APPEND flag is necessary.
 * Write a program that takes up to three command-line arguments:
 *
 * $ atomic_append filename num-bytes [x]
 *
 * This program should open the specified filename (creating it if necessary) and append num-bytes bytes to the file by using write() to write a byte at a time.
 * By default, the program should open the file with the O_APPEND flag,
 * but if a third command-line argument (x) is supplied, then the O_APPEND flag should be omitted,
 * and instead the program should perform an lseek(fd, 0, SEEK_END) call before each write().
 * Run two instances of this program at the same time without the x argument to write 1 million bytes to the same file:
 *
 * $ atomic_append f1 1000000 & atomic_append f1 1000000
 *
 * Repeat the same steps, writing to a different file, but this time specifying the x argument:
 *
 * $ atomic_append f2 1000000 x & atomic_append f2 1000000 x
 *
 * List the sizes of the files f1 and f2 using ls -l and explain the difference.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
  long int num_bytes, byte_count;
  int fd, flag_x = 0;
  char** endptr;

  if(argc < 3) {
    fprintf(stderr, "atomic_append filename num_bytes [x]\n");
    exit(EXIT_FAILURE);
  }

  num_bytes = strtol(argv[2], endptr, 10);
  if(**endptr != 0) {
    fprintf(stderr, "Invalid number of bytes\n");
    exit(EXIT_FAILURE);
  }

  if(argv[3] == NULL)
    fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
  else if(!strcmp(argv[3], "x")) {
    fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    flag_x = 1;
  }
  else {
    fprintf(stderr, "Invalid option in the third argument\n");
    exit(EXIT_FAILURE);
  }

  for(byte_count = 0; byte_count < num_bytes; byte_count++) {
    if(flag_x)
      lseek(fd, 0, SEEK_END);
    write(fd, " ", 1);
  }

  return 0;
}

/* Result:
 * -rw------- 1 xregreh xregreh 2000000 Set  6 17:17 f1
 * -rw------- 1 xregreh xregreh 1834171 Set  6 17:18 f2
 * That is, we LOST data when seek and write were not done atomically.
 *
 * According to the book:
 * "If the first process executing the code is interrupted between the lseek() and write() calls by a second process doing the same thing,
 * then both processes will set their file offset to the same location before writing, and when the first process is rescheduled,
 * it will overwrite the data already written by the second process."
 */
