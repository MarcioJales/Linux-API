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

int main(int argc, char** argv)
{

}
