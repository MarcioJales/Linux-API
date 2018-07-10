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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE          512
#define MAX_NUMBER_FILES  12

void parse_args(int argc, char** argv)
{
  /* After getopt scans argv, "-a" becomes argv[1] */
  int opt = getopt(argc, argv, "a");

  while(opt != -1) {
    if (opt == '?')
      exit(EXIT_FAILURE);

    opt = getopt(argc, argv, "a");
  }
};

int main(int argc, char** argv)
{
  char buffer[BUF_SIZE];
  int bytes_read = -1, file_count = 0, have_files = 0;
  int count, fd[MAX_NUMBER_FILES];

  parse_args(argc, argv);

  if((argc > 2 || (argc == 2 && optind == 1)))
    have_files = 1;

  if(have_files) {
    if(argc > MAX_NUMBER_FILES + optind) {
      printf("Too many files. Maximum of %d files.\n", MAX_NUMBER_FILES);
      exit(EXIT_FAILURE);
    }

    for(count = optind; count < argc; count++) {
      if(optind == 1)
        fd[file_count++] = open(argv[count], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
      if(optind == 2)
        fd[file_count++] = open(argv[count], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    }
  }

  /* optind = 1 means that no options were found */
  while(bytes_read) {
    bytes_read = read(STDIN_FILENO, buffer, BUF_SIZE);

    if(have_files) {
      file_count = 0;
      for(count = optind; count < argc ; count++)
        write(fd[file_count++], buffer, bytes_read);
    }
    write(STDOUT_FILENO, buffer, bytes_read);
  }

  if(have_files) {
    file_count = 0;
    for(count = optind; count < argc ; count++)
      close(fd[file_count++]);
  }
}
