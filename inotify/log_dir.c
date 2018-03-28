/* Chapter 19, exercise 1.
 *
 * Write a program that logs all file creations, deletions, and renames under
 * the directory named in its command-line argument. The program should monitor
 * events in all of the subdirectories under the specified directory. To obtain
 * a list of all of these subdirectories, you will need to make use of
 * nftw() (File Tree Walking: nftw()). When a new
 * subdirectory is added under the tree or a directory is deleted, the set of
 * monitored subdirectories should be updated accordingly.
 */

  /* IN PROGRESS */ 

 #include <sys/inotify.h>
 #include <limits.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdio.h>
 #include <errno.h>
 #define _XOPEN_SOURCE 500
 #include <ftw.h>

 /* This value is defined in /proc/sys/fs/inotify/max_user_watches
  * and is hardcoded here
  */
 #define MAX_USER_WATCHES 8192
 #define BUF_LEN (12 * (sizeof(struct inotify_event) + NAME_MAX + 1))

 int addWatch(const char *pathname, const struct stat *file_info, int type, struct FTW *sftw)
 {
   printf("%s\n", pathname);
   return 0;
 }

 int main(int argc, char **argv)
 {
   int instance_fd, watch_fd[MAX_USER_WATCHES];
   char buffer[BUF_LEN];
   int wfd_count = 0;

   if(argc < 2) {
     fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
     exit(EXIT_FAILURE);
   }

   instance_fd = inotify_init();
   if(instance_fd == -1) {
     fprintf(stderr, "Error (inotify_init): %s\n", strerror(errno));
     exit(EXIT_FAILURE);
   }

   if(nftw(argv[1], addWatch, 10, 0) == -1) {
     fprintf(stderr, "Error (nftw): %s\n", strerror(errno));
     exit(EXIT_FAILURE);
   }

   return 0;
 }
