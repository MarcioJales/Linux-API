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

 #include <sys/inotify.h>
 #include <limits.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdio.h>
 #include <errno.h>
 #define _XOPEN_SOURCE 500
 #include <ftw.h>

 #define BUF_LEN (12 * (sizeof(struct inotify_event) + NAME_MAX + 1))

 int instance_fd, watch_fd;
 char *dirpath;

 int addWatch(const char *pathname, const struct stat *file_info, int type, struct FTW *sftw)
 {
   int flags = IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE | IN_MOVE_SELF;

   if(type == FTW_D) {
     watch_fd = inotify_add_watch(instance_fd, pathname, flags);
     printf("Watch [wfd = %d] added to %s\n", watch_fd, pathname);
   }

   return 0;
 }

 void displayInotifyEvent(struct inotify_event *ievent)
 {
     printf("    wd = %2d; ", ievent->wd);
     if (ievent->cookie > 0)
         printf("cookie = %4d; ", ievent->cookie);

     printf("mask = ");
     if (ievent->mask & IN_DELETE)        printf("IN_DELETE ");
     if (ievent->mask & IN_IGNORED)       printf("IN_IGNORED ");
     if (ievent->mask & IN_ISDIR)         printf("IN_ISDIR ");
     if (ievent->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
     if (ievent->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
     if (ievent->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
     if (ievent->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
     if (ievent->mask & IN_CREATE)        printf("IN_CREATE ");

     printf("\n");

     if (ievent->len > 0)
       printf("        name = %s\n", ievent->name);

     if ((ievent->mask & IN_CREATE) && (ievent->mask & IN_ISDIR)) {
       if(nftw(dirpath, addWatch, 10, 0) == -1) {
         fprintf(stderr, "Error (nftw): %s\n", strerror(errno));
         exit(EXIT_FAILURE);
       }
     }
 }

 int main(int argc, char **argv)
 {
   char buffer[BUF_LEN];
   ssize_t bytes_read;
   char *event_pointer;
   struct inotify_event *event;

   if(argc < 2) {
     fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
     exit(EXIT_FAILURE);
   }

   dirpath = argv[1];

   instance_fd = inotify_init();
   if(instance_fd == -1) {
     fprintf(stderr, "Error (inotify_init): %s\n", strerror(errno));
     exit(EXIT_FAILURE);
   }

   if(nftw(dirpath, addWatch, 10, 0) == -1) {
     fprintf(stderr, "Error (nftw): %s\n", strerror(errno));
     exit(EXIT_FAILURE);
   }

   while(1) {
     bytes_read = read(instance_fd, buffer, BUF_LEN);
     if(bytes_read == 0)
       printf("Nothing to read\n");
     if(bytes_read == -1) {
       fprintf(stderr, "Error (read): %s\n", strerror(errno));
       exit(EXIT_FAILURE);
     }
     printf("Read %ld bytes from inotify fd\n", (long) bytes_read);

     for(event_pointer = buffer; event_pointer < buffer + bytes_read; ) {
       event = (struct inotify_event *) event_pointer;
       displayInotifyEvent(event);
       event_pointer += sizeof(struct inotify_event) + event->len;
     }
   }

   return 0;
 }
