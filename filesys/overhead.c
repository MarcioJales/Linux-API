/* Chapter 14, exercise 1
 *
 * Write a program that measures the time required to create and then remove a large number of 1-byte files from a single directory. 
 * The program should create files with names of the form xNNNNNN, where NNNNNN is replaced by a random six-digit number. 
 * The files should be created in the random order in which their names are generated, and then deleted in increasing numerical order (i.e., an order that is different from that in which they were created). 
 * The number of files (NF) and the directory in which they are to be created should be specifiable on the command line. 
 * Measure the times required for different values of NF (e.g., in the range from 1000 to 20,000) and for different file systems (e.g., ext2, ext3, and XFS). 
 * What patterns do you observe on each file system as NF increases? 
 * How do the various file systems compare? 
 * Do the results change if the files are created in increasing numerical order (x000001, x000001, x0000002, and so on) and then deleted in the same order?
 * If so, what do you think the reason(s) might be? Again, do the results vary across file-system types?
 */

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_PATH_SIZE 200

char* gen_number()
{
   int count, randnum;
   char* retstr = (char*) malloc(6);

   for(count = 0; count < 6; count++) {
      randnum = rand() % 10;
      sprintf(&retstr[count], "%d", randnum);
   }

   return retstr;
}

int main(int argc, char** argv)
{
   struct timeval start, end;
   int count, nf, fd;
   char* path_to_dir = (char*) malloc(MAX_PATH_SIZE + 1);
   char* filename = (char*) malloc(8);

   if(argc != 3) {
      printf("Usage: path/to/program number_of_files directory\n");
      exit(-1);
   }
   if(strlen(argv[2]) > MAX_PATH_SIZE) {
      printf("Path to directory has exceeded %d characters. Aborting\n", MAX_PATH_SIZE);
      exit(-1);
   }

   nf = atoi(argv[1]);
   strcpy(path_to_dir, argv[2]);
  
   chdir(path_to_dir);

   srand(time(NULL));   

   gettimeofday(&start, NULL);  

   // Loop to assign the file names and open them
   for(count = 0; count < nf; count++) {
      filename[0] = 'x';
      strcat(filename, (const char *) gen_number());
      
      fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
      write(fd, filename, 1);
      close(fd);

      memset(filename, 0, 8);
   }

   gettimeofday(&end, NULL);
   printf("Time to create = %f seconds\n", (double)end.tv_sec + (double)end.tv_usec/1000000 - (double)start.tv_sec - (double)start.tv_usec/1000000);
}
