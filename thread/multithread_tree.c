/*  Chapter 30, Exercise 2
 *
    Implement a set of thread-safe functions that update and search an unbalanced binary tree. This library should include functions (with the obvious purposes) of the following form:

    initialize(tree);
    add(tree, char *key, void *value);
    delete(tree, char *key)
    Boolean lookup(char *key, void **value)

    In the above prototypes, tree is a structure that points to the root of the tree (you will need to define a suitable structure or this purpose). Each element of the tree holds a key-value pair. You will also need to define the structure for each element to include a mutex that protects that element so that only one thread at a time can access it. The initialize(), add(), and lookup() functions are relatively simple to implement. The delete() operation requires a little more effort.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define NOTFOUND  0
#define FOUND     1

typedef struct keyPair {
  char key;
  short value;
  pthread_mutex_t mtx;
} element;

typedef struct binaryTree {
  struct binaryTree *left;
  struct binaryTree *right;
  element kv;
} tree;

void initialize(tree *t)
{
  int err;

  if(DEBUG)
    printf("Initializing tree (key = 0)...\n");

  t->left = NULL;
  t->right = NULL;
  (t->kv).key = 0;
  (t->kv).value = 88;
  if(err = pthread_mutex_init(&(t->kv).mtx, NULL)) {
    fprintf(stderr, "(err = %d) Failed to initialize mutex. Exiting...\n", err);
    exit(EXIT_FAILURE);
  }

  if(DEBUG)
    printf("Sucessfully initialized the tree.\n");
};

void add(tree *t, char *key, void *value)
{

};

void delete(tree *t, char *key)
{

};

char lookup(char *key, void **value)
{

};

int main(int argc, char **argv)
{
  short numThreads = 1;
  tree *root = (tree *) malloc(sizeof(tree));

  if(argc == 2)
    numThreads = (short) atoi(argv[1]);

  if(DEBUG)
    printf("Threads to create: %d\n", numThreads);

  initialize(root);

}
