/* Chapter 5, exercise 2
 *
 * Write a program that opens an existing file for writing with the O_APPEND flag, and then seeks to the beginning of the file before writing some data.
 * Where does the data appear in the file? Why?
 */

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
   int fd;
   char * str = "wrote after setting offset to the beginning of file opened with O_APPEND\n";

   fd = open("ch5ex2.txt", O_RDWR | O_APPEND);
   lseek(fd, 0, SEEK_SET);
   write(fd, str, strlen(str));
   close(fd);

   return 0;
}

/* As specified in the book:
 * if O_APPEND is given, writes are always appended to end of file
 */
