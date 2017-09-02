/* Chapter 4, exercise 1:
 *
 * The tee command reads its standard input until end-of-file, writing a
 * copy of the input to standard output and to the file named in its
 * command-line argument.  (We show an example of the use of this command
 * hwne we discuss FIFOs in Section 44.7.)  Implement tee using I/O
 * system calls.  By default, tee overwites any existing file with the
 * given name.  Impelment the -a command-line option (tee -a file), which
 * casuses te to append text to the end of a file  if it already exists.
 * (Refer to Appendix B for desription ofthe getopt() function, which can
 * be used to parse command-line options.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 128

int parse_args(int argc, char** argv)
{

};

int main(int argc, char** argv)
{
  char buffer[BUF_SIZE];
  int bytes_read = -1;

  /* That is: no files provided */
  if(argc < 3) {
    while(bytes_read) {
      bytes_read = read(STDIN_FILENO, buffer, BUF_SIZE);
      write(STDOUT_FILENO, buffer, bytes_read);
    }
  }
}
